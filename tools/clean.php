<?php

declare(strict_types=1);

$root = dirname(__DIR__);
$autoload = $root . DIRECTORY_SEPARATOR . 'vendor' . DIRECTORY_SEPARATOR . 'autoload.php';
if (!is_file($autoload)) {
    fwrite(STDERR, "Composer dependencies are missing. Run composer install first.\n");
    exit(1);
}

require $autoload;

use kodiak\bedrockprotocol\generator\configuration\GeneratorConfiguration;

/** Remove empty generated directories recursively, without touching files. */
function removeEmptyDirectories(string $directory): int
{
    if (!is_dir($directory) || is_link($directory)) {
        return 0;
    }

    $removed = 0;
    foreach (array_diff(scandir($directory) ?: [], ['.', '..']) as $entry) {
        $child = $directory . DIRECTORY_SEPARATOR . $entry;
        if (is_dir($child) && !is_link($child)) {
            $removed += removeEmptyDirectories($child);
        }
    }

    $entries = array_diff(scandir($directory) ?: [], ['.', '..']);
    if ($entries === [] && rmdir($directory)) {
        ++$removed;
    }
    return $removed;
}

try {
    $configuration = GeneratorConfiguration::fromArguments($root, array_slice($argv, 1));
    $output = rtrim($configuration->outputDirectory, DIRECTORY_SEPARATOR);
    $rootPath = realpath($root);
    if ($rootPath === false) {
        throw new RuntimeException('Unable to resolve the project root.');
    }

    $normalizedRoot = strtolower(rtrim(str_replace(['/', '\\'], DIRECTORY_SEPARATOR, $rootPath), DIRECTORY_SEPARATOR));
    $normalizedOutput = strtolower(rtrim(str_replace(['/', '\\'], DIRECTORY_SEPARATOR, $output), DIRECTORY_SEPARATOR));
    $rootPrefix = $normalizedRoot . DIRECTORY_SEPARATOR;
    if ($normalizedOutput !== $normalizedRoot && !str_starts_with($normalizedOutput, $rootPrefix)) {
        throw new RuntimeException('Refusing to clean an output directory outside the project root.');
    }

    $manifestPath = $output . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . '.generated-manifest.json';
    if (!is_file($manifestPath)) {
        fwrite(STDOUT, "no generated manifest found; nothing to clean\n");
        exit(0);
    }

    $contents = file_get_contents($manifestPath);
    if ($contents === false) {
        throw new RuntimeException('Unable to read generated manifest: ' . $manifestPath);
    }
    $manifest = json_decode($contents, true, 512, JSON_THROW_ON_ERROR);
    $files = is_array($manifest['files'] ?? null) ? $manifest['files'] : [];
    // Older manifests stored generator-owned adapters in runtimeFiles. Keep
    // removing that field for one-way cleanup, while supportFiles are library
    // sources and must remain available for the next generation.
    $legacyRuntimeFiles = is_array($manifest['runtimeFiles'] ?? null) ? $manifest['runtimeFiles'] : [];
    $files += $legacyRuntimeFiles;
    $removed = 0;
    foreach (array_keys($files) as $relative) {
        if (!is_string($relative) || $relative === '' || preg_match('~^(?:[A-Za-z]:[\\\\/]|[\\\\/]{2})~', $relative) === 1) {
            throw new RuntimeException('Unsafe generated path in manifest: ' . (string) $relative);
        }
        $relative = str_replace(['/', '\\'], DIRECTORY_SEPARATOR, $relative);
        if (preg_match('/(?:^|[\\\\])\.\.(?:[\\\\]|$)/', $relative) === 1) {
            throw new RuntimeException('Unsafe generated path in manifest: ' . $relative);
        }
        $path = $output . DIRECTORY_SEPARATOR . $relative;
        if (is_file($path) && unlink($path)) {
            ++$removed;
        }
    }

    if (is_file($manifestPath) && unlink($manifestPath)) {
        ++$removed;
    }

    $emptyDirectories = [
        $output . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock' . DIRECTORY_SEPARATOR . 'protocol' . DIRECTORY_SEPARATOR . 'types',
        $output . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock' . DIRECTORY_SEPARATOR . 'protocol' . DIRECTORY_SEPARATOR . 'pool',
        $output . DIRECTORY_SEPARATOR . 'metadata',
    ];
    foreach ($emptyDirectories as $directory) {
        $removed += removeEmptyDirectories($directory);
    }

    printf("cleaned %d generated files\n", $removed);
} catch (Throwable $error) {
    fwrite(STDERR, 'clean failed: ' . $error->getMessage() . PHP_EOL);
    exit(1);
}
