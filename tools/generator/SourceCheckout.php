<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator;

use RuntimeException;

class SourceCheckout
{
    /** @return array{root: string, revision: string, commitDate: string, source: string, temporary: bool} */
    public function resolve(Configuration $configuration): array
    {
        $temporaryRoot = rtrim(sys_get_temp_dir(), DIRECTORY_SEPARATOR)
            . DIRECTORY_SEPARATOR
            . 'bedrock-protocol-source-'
            . bin2hex(random_bytes(8));

        try {
            $clone = $this->runGit([
                'clone',
                '--depth',
                '1',
                '--single-branch',
                '--branch',
                $configuration->sourceRef,
                $configuration->sourceUrl,
                $temporaryRoot,
            ]);
            if ($clone['exitCode'] !== 0) {
                $this->removeDirectory($temporaryRoot);
                $this->makeDirectory($temporaryRoot);
                $this->mustRunGit(['init', $temporaryRoot]);
                $this->mustRunGit(['-C', $temporaryRoot, 'remote', 'add', 'origin', $configuration->sourceUrl]);
                $this->mustRunGit(['-C', $temporaryRoot, 'fetch', '--depth', '1', 'origin', $configuration->sourceRef]);
                $this->mustRunGit(['-C', $temporaryRoot, 'checkout', '--detach', 'FETCH_HEAD']);
            }

            if (!$this->isProtocolSource($temporaryRoot)) {
                throw new RuntimeException('The fetched repository does not contain a src/ directory: ' . $configuration->sourceUrl);
            }

            return [
                'root' => $temporaryRoot,
                'revision' => $this->git($temporaryRoot, ['rev-parse', 'HEAD']),
                'commitDate' => $this->commitDate($temporaryRoot),
                'source' => 'fresh-git-checkout',
                'temporary' => true,
            ];
        } catch (\Throwable $error) {
            $this->removeDirectory($temporaryRoot);
            throw $error;
        }
    }

    /** @param array{root: string, temporary?: bool} $source */
    public function cleanup(array $source): void
    {
        if (($source['temporary'] ?? false) !== true) {
            return;
        }
        $root = $source['root'];
        $tempRoot = rtrim(sys_get_temp_dir(), DIRECTORY_SEPARATOR) . DIRECTORY_SEPARATOR;
        if (!str_starts_with($root, $tempRoot) || !str_contains(basename($root), 'bedrock-protocol-source-')) {
            throw new RuntimeException('Refusing to remove a non-generator temporary checkout: ' . $root);
        }
        $this->removeDirectory($root);
    }

    private function isProtocolSource(string $directory): bool
    {
        return is_dir($directory . DIRECTORY_SEPARATOR . 'src')
            && is_file($directory . DIRECTORY_SEPARATOR . 'composer.json');
    }

    private function git(string $directory, array $arguments): string
    {
        array_unshift($arguments, '-C', $directory);
        return trim($this->mustRunGit($arguments));
    }

    private function commitDate(string $directory): string
    {
        $raw = $this->git($directory, ['show', '-s', '--format=raw', 'HEAD']);
        if (preg_match('/^committer .*? (\d+) ([+-]\d{4})$/m', $raw, $matches) !== 1) {
            return '';
        }
        $date = new \DateTimeImmutable('@' . $matches[1]);
        return $date->setTimezone(new \DateTimeZone($matches[2]))->format(DATE_ATOM);
    }

    private function mustRunGit(array $arguments): string
    {
        $result = $this->runGit($arguments);
        if ($result['exitCode'] !== 0) {
            throw new RuntimeException('git failed (' . $result['exitCode'] . '): ' . $result['output']);
        }
        return $result['output'];
    }

    /** @return array{exitCode: int, output: string} */
    private function runGit(array $arguments): array
    {
        $command = 'git';
        foreach ($arguments as $argument) {
            $command .= ' ' . escapeshellarg((string) $argument);
        }
        $pipes = [];
        $process = proc_open($command, [1 => ['pipe', 'w'], 2 => ['pipe', 'w']], $pipes);
        if (!is_resource($process)) {
            throw new RuntimeException('Unable to start git.');
        }
        $output = stream_get_contents($pipes[1]) . stream_get_contents($pipes[2]);
        fclose($pipes[1]);
        fclose($pipes[2]);
        return ['exitCode' => proc_close($process), 'output' => trim($output)];
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
                @chmod($item->getPathname(), 0777);
                rmdir($item->getPathname());
            } else {
                @chmod($item->getPathname(), 0666);
                unlink($item->getPathname());
            }
        }
        @chmod($directory, 0777);
        rmdir($directory);
    }
}
