<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\emitter;

use kodiak\bedrockprotocol\generator\Configuration;
use kodiak\bedrockprotocol\generator\CPPName;
use kodiak\bedrockprotocol\generator\CPP\CPPDefaultValueResolver;
use RuntimeException;

trait PathEmissionMethods
{
    private function typePath(string $output, string $name, ?array $declaration = null): string
    {
        $relative = $declaration === null
            ? 'types/' . $name . '.hpp'
            : $this->generatedTypeInclude($declaration);
        return $output
            . DIRECTORY_SEPARATOR . 'include'
            . DIRECTORY_SEPARATOR . 'bedrock'
            . DIRECTORY_SEPARATOR . 'protocol'
            . DIRECTORY_SEPARATOR . str_replace('/', DIRECTORY_SEPARATOR, $relative);
    }

    private function generatedTypeName(array $declaration): string
    {
        $name = CPPName::type((string) ($declaration['name'] ?? ''));
        $alias = $this->catalog->typeAlias($name);
        return $alias !== null && preg_match('/^[A-Za-z_][A-Za-z0-9_]*$/', $alias) === 1
            ? $alias
            : $name;
    }

    private function generatedTypeInclude(array $declaration): string
    {
        $file = str_replace('\\', '/', (string) ($declaration['file'] ?? ''));
        $relative = '';
        if (str_starts_with($file, 'src/types/')) {
            $relative = substr($file, strlen('src/types/'));
        }
        if ($relative === '') {
            $relative = $this->generatedTypeName($declaration) . '.php';
        }
        $relative = preg_replace('/\.php$/i', '.hpp', $relative) ?? $relative;
        $directory = dirname($relative);
        $name = $this->generatedTypeName($declaration) . '.hpp';
        return $directory === '.'
            ? 'types/' . $name
            : 'types/' . trim(str_replace('\\', '/', $directory), '/') . '/' . $name;
    }

    private function runtimeTypePath(string $output, array $declaration, string $name): string
    {
        $include = $this->generatedTypeInclude($declaration);
        $include = preg_replace('~[^/]+$~', $name . '.hpp', $include) ?? $include;
        return $output
            . DIRECTORY_SEPARATOR . 'include'
            . DIRECTORY_SEPARATOR . 'bedrock'
            . DIRECTORY_SEPARATOR . 'protocol'
            . DIRECTORY_SEPARATOR . str_replace('/', DIRECTORY_SEPARATOR, $include);
    }

    private function initializeTypeIncludePaths(array $declarations): void
    {
        $this->typeIncludePaths = [];
        $byName = [];
        $runtimeTypeNames = [
            'MetadataProperty',
            'EntityMetadataCollection',
            'EntityMetadataTypes',
            'ItemStack',
            'ItemStackWrapper',
            'AbilitiesData',
            'AbilitiesLayer',
            'PropertySyncData',
            'PlayerAuthInputFlags',
            'PlayerBlockAction',
        ];
        foreach ($declarations as $declaration) {
            $sourceName = CPPName::type((string) ($declaration['name'] ?? ''));
            if ($sourceName === '') {
                continue;
            }
            if (!$this->isGeneratedType($declaration)
                && !in_array($sourceName, $runtimeTypeNames, true)
                && !str_ends_with($sourceName, 'MetadataProperty')) {
                $byName[$sourceName] = $declaration;
                continue;
            }
            $include = $this->generatedTypeInclude($declaration);
            $this->typeIncludePaths[$sourceName] = $include;
            $this->typeIncludePaths[$this->generatedTypeName($declaration)] = $include;
            $byName[$sourceName] = $declaration;
        }

        if (isset($byName['MetadataProperty'])) {
            $include = $this->runtimeTypeInclude($byName['MetadataProperty'], 'MetadataProperty');
            foreach ($byName as $name => $declaration) {
                if ($name === 'MetadataProperty'
                    || $name === 'EntityMetadataCollection'
                    || $name === 'EntityMetadataTypes'
                    || str_ends_with($name, 'MetadataProperty')) {
                    $this->typeIncludePaths[$name] = $include;
                }
            }
            $this->typeIncludePaths['EntityMetadataType'] = $include;
        }
        if (isset($byName['ItemStack'])) {
            $this->typeIncludePaths['ItemStack'] = $this->runtimeTypeInclude($byName['ItemStack'], 'NetworkItemDescriptor');
            $this->typeIncludePaths['NetworkItemDescriptor'] = $this->typeIncludePaths['ItemStack'];
        }
        if (isset($byName['ItemStackWrapper'])) {
            $this->typeIncludePaths['ItemStackWrapper'] = $this->runtimeTypeInclude($byName['ItemStackWrapper'], 'NetworkItemStack');
            $this->typeIncludePaths['NetworkItemStack'] = $this->typeIncludePaths['ItemStackWrapper'];
        }
    }

    private function runtimeTypeInclude(array $declaration, string $name): string
    {
        $include = $this->generatedTypeInclude($declaration);
        return preg_replace('~[^/]+$~', $name . '.hpp', $include) ?? $include;
    }

    private function includeForType(string $type): ?string
    {
        $short = CPPName::type($type);
        if (isset($this->typeIncludePaths[$short])) {
            return $this->typeIncludePaths[$short];
        }
        $alias = $this->catalog->typeAlias($short);
        if ($alias !== null && preg_match('/^[A-Za-z_][A-Za-z0-9_]*$/', $alias) === 1) {
            if (isset($this->typeIncludePaths[$alias])) {
                return $this->typeIncludePaths[$alias];
            }
        }
        return $this->resolveTypeInclude($this->catalog->includeFor($type));
    }

    private function resolveTypeInclude(?string $include): ?string
    {
        if ($include === null) {
            return null;
        }
        $include = str_replace('\\', '/', $include);
        if (!str_starts_with($include, 'types/')) {
            return $include;
        }
        $name = basename($include, '.hpp');
        return $this->typeIncludePaths[$name] ?? $include;
    }
}
