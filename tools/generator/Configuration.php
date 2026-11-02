<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator;

use InvalidArgumentException;
use JsonException;

class Configuration
{
    public function __construct(
        public readonly string $sourceUrl,
        public readonly string $sourceRef,
        public readonly string $outputDirectory,
        public readonly string $configPath,
        /** @var array<string, mixed> */
        public readonly array $translation = [],
    ) {
    }

    /** @param list<string> $arguments */
    public static function fromArguments(string $root, array $arguments): self
    {
        $options = self::arguments($arguments);
        $configPath = self::absolutePath($root, (string) ($options['config'] ?? 'generator.json'));
        $config = self::readJsonConfig($configPath);

        $source = is_array($config['source'] ?? null) ? $config['source'] : [];
        $output = $config['output'] ?? '.';
        if (is_array($output)) {
            $output = $output['directory'] ?? '.';
        }

        $sourceUrl = (string) ($options['url'] ?? $source['url'] ?? 'https://github.com/axolotl-pm/BedrockProtocol.git');
        $sourceRef = (string) ($options['ref'] ?? $source['ref'] ?? 'master');
        if ($sourceUrl === '' || $sourceRef === '') {
            throw new InvalidArgumentException('source.url and source.ref must not be empty.');
        }

        return new self(
            $sourceUrl,
            $sourceRef,
            self::absolutePath($root, (string) ($options['output'] ?? $output)),
            $configPath,
            is_array($config['translation'] ?? null) ? $config['translation'] : [],
        );
    }

    /** @param list<string> $arguments @return array<string, string> */
    private static function arguments(array $arguments): array
    {
        $options = [];
        $known = ['config', 'url', 'ref', 'output'];
        foreach ($arguments as $argument) {
            if (!str_starts_with($argument, '--')) {
                throw new InvalidArgumentException('Unknown argument: ' . $argument);
            }
            $argument = substr($argument, 2);
            if (!str_contains($argument, '=')) {
                throw new InvalidArgumentException('Expected --name=value, got --' . $argument);
            }
            [$name, $value] = explode('=', $argument, 2);
            if (!in_array($name, $known, true)) {
                throw new InvalidArgumentException('Unknown option: --' . $name);
            }
            $options[$name] = $value;
        }
        return $options;
    }

    /** @return array<string, mixed> */
    private static function readJsonConfig(string $path): array
    {
        if (!is_file($path)) {
            return [];
        }
        $contents = file_get_contents($path);
        if ($contents === false) {
            throw new InvalidArgumentException('Unable to read generator config: ' . $path);
        }
        try {
            $config = json_decode($contents, true, 512, JSON_THROW_ON_ERROR);
        } catch (JsonException $error) {
            throw new InvalidArgumentException('Invalid JSON in generator config: ' . $error->getMessage(), 0, $error);
        }
        if (!is_array($config)) {
            throw new InvalidArgumentException('Generator config must contain a JSON object: ' . $path);
        }
        return $config;
    }

    private static function absolutePath(string $root, string $path): string
    {
        if ($path === '') {
            throw new InvalidArgumentException('Configured path must not be empty.');
        }
        if (!self::isAbsolute($path)) {
            $path = $root . DIRECTORY_SEPARATOR . $path;
        }
        return self::normalizePath($path);
    }

    private static function isAbsolute(string $path): bool
    {
        return preg_match('~^(?:[A-Za-z]:[\\\\/]|[\\\\/]{2})~', $path) === 1;
    }

    private static function normalizePath(string $path): string
    {
        $path = str_replace(['/', '\\'], DIRECTORY_SEPARATOR, $path);
        $prefix = '';
        if (preg_match('/^[A-Za-z]:\\\\/', $path) === 1) {
            $prefix = substr($path, 0, 3);
            $path = substr($path, 3);
        } elseif (str_starts_with($path, DIRECTORY_SEPARATOR . DIRECTORY_SEPARATOR)) {
            $prefix = DIRECTORY_SEPARATOR . DIRECTORY_SEPARATOR;
            $path = ltrim(substr($path, 2), DIRECTORY_SEPARATOR);
        } elseif (str_starts_with($path, DIRECTORY_SEPARATOR)) {
            $prefix = DIRECTORY_SEPARATOR;
            $path = ltrim($path, DIRECTORY_SEPARATOR);
        }

        $parts = [];
        foreach (explode(DIRECTORY_SEPARATOR, $path) as $part) {
            if ($part === '' || $part === '.') {
                continue;
            }
            if ($part === '..') {
                if ($parts !== [] && end($parts) !== '..') {
                    array_pop($parts);
                } else {
                    $parts[] = $part;
                }
                continue;
            }
            $parts[] = $part;
        }
        return rtrim($prefix . implode(DIRECTORY_SEPARATOR, $parts), DIRECTORY_SEPARATOR);
    }
}
