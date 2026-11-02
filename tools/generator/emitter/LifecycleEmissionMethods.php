<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\emitter;

use kodiak\bedrockprotocol\generator\Configuration;
use kodiak\bedrockprotocol\generator\CPPName;
use kodiak\bedrockprotocol\generator\CPP\CPPDefaultValueResolver;
use RuntimeException;

trait LifecycleEmissionMethods
{
    public static function cachedStats(Configuration $configuration, array $source): ?array
    {
        $manifestPath = self::manifestPath($configuration->outputDirectory);
        if (!is_file($manifestPath)) {
            return null;
        }
        $contents = file_get_contents($manifestPath);
        if ($contents === false) {
            return null;
        }
        try {
            $manifest = json_decode($contents, true, 512, JSON_THROW_ON_ERROR);
        } catch (\JsonException) {
            return null;
        }
        if (!is_array($manifest) || ($manifest['schema'] ?? null) !== 2) {
            return null;
        }
        if (($manifest['configurationFingerprint'] ?? null) !== self::configurationFingerprint($configuration)) {
            return null;
        }
        if (($manifest['generatorFingerprint'] ?? null) !== self::generatorFingerprint()) {
            return null;
        }
        $manifestSource = $manifest['source'] ?? [];
        if (!is_array($manifestSource)
            || ($manifestSource['url'] ?? null) !== $configuration->sourceUrl
            || ($manifestSource['ref'] ?? null) !== $configuration->sourceRef
            || ($manifestSource['revision'] ?? null) !== $source['revision']) {
            return null;
        }
        if (!self::supportFilesMatch($configuration->outputDirectory, $manifest['supportFiles'] ?? [])) {
            return null;
        }
        foreach (self::manifestFiles($manifest) as $relative => $metadata) {
            $path = self::safeOutputPath($configuration->outputDirectory, $relative);
            if (!is_file($path) || !is_array($metadata) || ($metadata['sha256'] ?? null) !== hash_file('sha256', $path)) {
                return null;
            }
        }
        $stats = $manifest['stats'] ?? null;
        if (!is_array($stats)) {
            return null;
        }
        $stats['changed'] = 0;
        $stats['removed'] = 0;
        $stats['cached'] = 1;
        return $stats;
    }

    public function emit(array $model, array $source, Configuration $configuration): array
    {
        $output = rtrim($configuration->outputDirectory, DIRECTORY_SEPARATOR);
        $this->outputRoot = $output;
        $this->emittedFiles = [];
        $this->changedFiles = [];
        $this->previousManifest = $this->readManifest($output);
        $base = $output . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock' . DIRECTORY_SEPARATOR . 'protocol';
        $this->makeDirectory($base);
        $this->makeDirectory($output . DIRECTORY_SEPARATOR . 'metadata');

        $declarations = $model['declarations'];
        $types = array_values(array_filter($declarations, fn (array $declaration): bool => $this->isGeneratedType($declaration)));
        $enums = array_values(array_filter($types, fn (array $declaration): bool => $this->isEnumDeclaration($declaration)));
        $classes = array_values(array_filter($types, fn (array $declaration): bool => !$this->isEnumDeclaration($declaration)));
        $this->enumNames = array_fill_keys(array_map(fn (array $enum): string => $this->generatedTypeName($enum), $enums), true);
        $this->initializeTypeIncludePaths($declarations);

        $this->writeRuntime($output);
        $this->writeGeneratedRuntimeTypes($output, $declarations);
        $this->writeForwardDeclarations($output, $types);
        $this->writePacketIds($output, $model['packets']);
        foreach ($enums as $declaration) {
            $this->writeEnum($output, $declaration);
        }
        foreach ($classes as $declaration) {
            $this->writeType($output, $declaration, $types);
        }
        $this->writeTypeDefinitions($output, $enums, $classes);
        foreach ($model['packets'] as $packet) {
            $this->writePacket($output, $packet, $types);
        }
        $this->writePacketPool($output, $model['registrations']);

        $removed = $this->removeStaleFiles($output);

        $protocolMetadata = [
            'source' => [
                'url' => $configuration->sourceUrl,
                'ref' => $configuration->sourceRef,
                'revision' => $source['revision'],
                'commitDate' => $source['commitDate'],
                'checkout' => $source['source'],
            ],
            'files' => $model['files'],
            'fileHashes' => $model['fileHashes'],
            'packets' => array_map(static fn (array $packet): array => [
                'name' => $packet['name'],
                'networkId' => $packet['networkId'],
                'source' => $packet['file'] . ':' . $packet['line'],
            ], $model['packets']),
            'declarations' => count($declarations),
            'enums' => count($enums),
            'poolRegistrations' => count($model['registrations']),
        ];
        $this->writeJson($output . DIRECTORY_SEPARATOR . 'metadata' . DIRECTORY_SEPARATOR . 'protocol.json', $protocolMetadata);
        $this->writeJson($output . DIRECTORY_SEPARATOR . 'metadata' . DIRECTORY_SEPARATOR . 'codegen.json', [
            'translator' => 'nikic/php-parser AST',
            'unsupported' => $this->translator->unsupported(),
            'notes' => [
                'Unsupported constructs are explicit adapter points and are never emitted as PHP source.',
                'Regenerate after changing the configured upstream ref.',
            ],
        ]);

        $stats = [
            'packets' => count($model['packets']),
            'declarations' => count($declarations),
            'enums' => count($enums),
            'registrations' => count($model['registrations']),
            'unsupported' => count($this->translator->unsupported()),
            'changed' => count($this->changedFiles),
            'removed' => $removed,
            'cached' => 0,
        ];
        $manifest = [
            'schema' => 2,
            'configurationFingerprint' => self::configurationFingerprint($configuration),
            'generatorFingerprint' => self::generatorFingerprint(),
            'source' => [
                'url' => $configuration->sourceUrl,
                'ref' => $configuration->sourceRef,
                'revision' => $source['revision'],
                'commitDate' => $source['commitDate'],
            ],
            'supportFiles' => $this->supportFiles($output),
            'inputFiles' => $model['fileHashes'],
            'files' => $this->fileManifest($output),
            'stats' => $stats,
        ];
        $this->writeJson(self::manifestPath($output), $manifest, false);

        return $stats;
    }
}
