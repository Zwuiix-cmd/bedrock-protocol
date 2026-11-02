<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\emitter;

use kodiak\bedrockprotocol\generator\Configuration;
use kodiak\bedrockprotocol\generator\CPPName;
use kodiak\bedrockprotocol\generator\CPP\CPPDefaultValueResolver;
use RuntimeException;

trait ManifestEmissionMethods
{
    private static function manifestPath(string $output): string
    {
        return rtrim($output, DIRECTORY_SEPARATOR)
            . DIRECTORY_SEPARATOR
            . 'include'
            . DIRECTORY_SEPARATOR
            . '.generated-manifest.json';
    }

    private static function configurationFingerprint(Configuration $configuration): string
    {
        $configHash = is_file($configuration->configPath) ? hash_file('sha256', $configuration->configPath) : null;
        return hash('sha256', json_encode([
            'sourceUrl' => $configuration->sourceUrl,
            'sourceRef' => $configuration->sourceRef,
            'outputDirectory' => $configuration->outputDirectory,
            'configHash' => $configHash,
            'translation' => $configuration->translation,
        ], JSON_THROW_ON_ERROR));
    }

    private static function generatorFingerprint(): string
    {
        $generatorRoot = dirname(__DIR__);
        $files = glob($generatorRoot . DIRECTORY_SEPARATOR . '*.php') ?: [];
        $files = array_merge(
            $files,
            glob($generatorRoot . DIRECTORY_SEPARATOR . '*/*.php') ?: [],
        );
        $files[] = dirname($generatorRoot) . DIRECTORY_SEPARATOR . 'generate.php';
        sort($files);
        $parts = [];
        foreach ($files as $file) {
            $hash = hash_file('sha256', $file);
            if ($hash === false) {
                throw new RuntimeException('Unable to hash generator file: ' . $file);
            }
            $parts[] = basename($file) . ':' . $hash;
        }
        return hash('sha256', implode("\n", $parts));
    }

    private function supportFiles(string $output): array
    {
        $result = [];
        foreach (self::requiredRuntimeFiles() as $relative) {
            $path = self::safeOutputPath($output, $relative);
            if (!is_file($path)) {
                throw new RuntimeException('Required library support file is missing: ' . $path);
            }
            $hash = hash_file('sha256', $path);
            if ($hash === false) {
                throw new RuntimeException('Unable to hash runtime file: ' . $path);
            }
            $result[$relative] = [
                'sha256' => $hash,
                'bytes' => (int) filesize($path),
            ];
        }
        return $result;
    }

    private static function supportFilesMatch(string $output, mixed $supportFiles): bool
    {
        if (!is_array($supportFiles)) {
            return false;
        }
        foreach (self::requiredRuntimeFiles() as $relative) {
            $metadata = $supportFiles[$relative] ?? null;
            $path = self::safeOutputPath($output, $relative);
            if (!is_array($metadata) || !is_file($path) || ($metadata['sha256'] ?? null) !== hash_file('sha256', $path)) {
                return false;
            }
        }
        return true;
    }

    private static function requiredRuntimeFiles(): array
    {
        return [
            'include/bedrock/protocol/io/BinaryStream.hpp',
            'include/bedrock/protocol/io/ReadOnlyBinaryStream.hpp',
            'include/bedrock/protocol/types/NetworkNbt.hpp',
        ];
    }

    private function readManifest(string $output): array
    {
        $path = self::manifestPath($output);
        if (!is_file($path)) {
            return [];
        }
        $contents = file_get_contents($path);
        if ($contents === false) {
            return [];
        }
        try {
            $manifest = json_decode($contents, true, 512, JSON_THROW_ON_ERROR);
        } catch (\JsonException) {
            return [];
        }
        return is_array($manifest) ? $manifest : [];
    }

    private static function manifestFiles(array $manifest): array
    {
        $files = $manifest['files'] ?? [];
        if (!is_array($files)) {
            return [];
        }
        $result = [];
        foreach ($files as $relative => $metadata) {
            if (is_string($relative) && is_array($metadata)) {
                $result[$relative] = $metadata;
            }
        }
        return $result;
    }

    private function fileManifest(string $output): array
    {
        $result = [];
        foreach (array_keys($this->emittedFiles) as $relative) {
            $path = self::safeOutputPath($output, $relative);
            if (!is_file($path)) {
                throw new RuntimeException('Generated file disappeared before manifest creation: ' . $path);
            }
            $hash = hash_file('sha256', $path);
            if ($hash === false) {
                throw new RuntimeException('Unable to hash generated file: ' . $path);
            }
            $result[$relative] = [
                'sha256' => $hash,
                'bytes' => (int) filesize($path),
            ];
        }
        ksort($result);
        return $result;
    }

    private function removeStaleFiles(string $output): int
    {
        $removed = 0;
        foreach (array_keys(self::manifestFiles($this->previousManifest)) as $relative) {
            if (isset($this->emittedFiles[$relative])) {
                continue;
            }
            $path = self::safeOutputPath($output, $relative);
            if (is_file($path) && unlink($path)) {
                ++$removed;
            }
        }
        return $removed;
    }

    private static function safeOutputPath(string $output, string $relative): string
    {
        $relative = str_replace(['/', '\\'], DIRECTORY_SEPARATOR, $relative);
        if ($relative === '' || self::isAbsolutePath($relative) || preg_match('/(?:^|[\\\\])\.\.(?:[\\\\]|$)/', $relative) === 1) {
            throw new RuntimeException('Unsafe generated path in manifest: ' . $relative);
        }
        return rtrim($output, DIRECTORY_SEPARATOR) . DIRECTORY_SEPARATOR . $relative;
    }

    private static function isAbsolutePath(string $path): bool
    {
        return preg_match('~^(?:[A-Za-z]:[\\\\/]|[\\\\/]{2})~', $path) === 1 || str_starts_with($path, DIRECTORY_SEPARATOR);
    }

    private function relativePath(string $path): string
    {
        $root = rtrim($this->outputRoot, DIRECTORY_SEPARATOR) . DIRECTORY_SEPARATOR;
        if (!str_starts_with($path, $root)) {
            throw new RuntimeException('Refusing to emit outside configured output: ' . $path);
        }
        return str_replace(DIRECTORY_SEPARATOR, '/', substr($path, strlen($root)));
    }

    private function writeJson(string $path, array $value, bool $track = true): void
    {
        $json = json_encode($value, JSON_PRETTY_PRINT | JSON_UNESCAPED_SLASHES | JSON_THROW_ON_ERROR) . PHP_EOL;
        $this->write($path, $json, $track);
    }

    private function write(string $path, string $contents, bool $track = true): void
    {
        $this->makeDirectory(dirname($path));
        if ($track) {
            $this->emittedFiles[$this->relativePath($path)] = true;
        }
        if (is_file($path) && file_get_contents($path) === $contents) {
            return;
        }
        if (file_put_contents($path, $contents) === false) {
            throw new RuntimeException('Unable to write ' . $path);
        }
        if ($track) {
            $this->changedFiles[$this->relativePath($path)] = true;
        }
    }

    private function makeDirectory(string $directory): void
    {
        if (!is_dir($directory) && !mkdir($directory, 0777, true) && !is_dir($directory)) {
            throw new RuntimeException('Unable to create directory: ' . $directory);
        }
    }

    private function removeDirectory(string $directory): void
    {
        if (!is_dir($directory)) {
            return;
        }
        $iterator = new \RecursiveIteratorIterator(
            new \RecursiveDirectoryIterator($directory, \FilesystemIterator::SKIP_DOTS),
            \RecursiveIteratorIterator::CHILD_FIRST,
        );
        foreach ($iterator as $item) {
            if ($item->isDir()) {
                rmdir($item->getPathname());
            } else {
                unlink($item->getPathname());
            }
        }
        rmdir($directory);
    }
}
