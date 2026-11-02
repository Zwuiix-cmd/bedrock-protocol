<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator;

use kodiak\bedrockprotocol\generator\catalog\CPPTranslationCatalogDefaults;

/**
 * Describes the small amount of C++ runtime knowledge needed while translating
 * the PHP source AST.
 *
 * Keeping this catalogue outside PhpToCPPTranslator is intentional: the
 * translator remains a source/AST translator and does not grow packet-specific
 * branches every time the upstream protocol changes. The catalogue can also be
 * overridden from generator.json when a library integration uses different C++
 * support types.
 */
class CPPTranslationCatalog
{
    /** @var array<string, string> */
    private array $typeAliases;

    /** @var array<string, list<string>> */
    private array $typeIncludes;

    /** @var array<string, array<string, mixed>> */
    private array $typeCodecs;

    /** @var array<string, true> */
    private array $runtimeDeclarations;

    /** @var array<string, true> */
    private array $knownValueTypes;

    /** @var array<string, true> */
    private array $serializerClasses;

    /** @var array<string, array<string, string>> */
    private array $streamMethods;

    /** @var array<string, array<string, array<string, string>>> */
    private array $specialCodecs;

    /** @var array<string, string> */
    private array $constructors;

    /** @var array<string, array<string, string>> */
    private array $constantAliases;

    /** @var array<string, array<string, string>> */
    private array $getterAliases;

    /** @var array<string, array<string, string>> */
    private array $methodAliases;

    /** @var array<string, array<string, string>> */
    private array $staticCallAliases;

    /** @var array<string, array<string, string>> */
    private array $valueWriters;

    /** @param array<string, mixed> $config */
    public function __construct(array $config = [])
    {
        $defaults = CPPTranslationCatalogDefaults::values();
        $config = self::normalize($config);

        $this->typeAliases = array_replace($defaults['typeAliases'], $config['typeAliases']);
        $this->typeIncludes = array_replace($defaults['typeIncludes'], $config['typeIncludes']);
        $this->typeCodecs = array_replace_recursive($defaults['typeCodecs'], $config['typeCodecs']);
        $this->runtimeDeclarations = array_fill_keys(
            array_values(array_unique(array_merge($defaults['runtimeDeclarations'], $config['runtimeDeclarations']))),
            true,
        );
        $this->knownValueTypes = array_fill_keys(
            array_values(array_unique(array_merge($defaults['knownValueTypes'], $config['knownValueTypes']))),
            true,
        );
        $this->serializerClasses = array_fill_keys(
            array_values(array_unique(array_merge($defaults['serializerClasses'], $config['serializerClasses']))),
            true,
        );
        $this->streamMethods = array_replace_recursive($defaults['streamMethods'], $config['streamMethods']);
        $this->specialCodecs = array_replace_recursive($defaults['specialCodecs'], $config['specialCodecs']);
        $this->constructors = array_replace($defaults['constructors'], $config['constructors']);
        $this->constantAliases = array_replace_recursive($defaults['constantAliases'], $config['constantAliases']);
        $this->getterAliases = array_replace_recursive($defaults['getterAliases'], $config['getterAliases']);
        $this->methodAliases = array_replace_recursive($defaults['methodAliases'], $config['methodAliases']);
        $this->staticCallAliases = array_replace_recursive($defaults['staticCallAliases'], $config['staticCallAliases']);
        $this->valueWriters = array_replace_recursive($defaults['valueWriters'], $config['valueWriters']);
    }

    public function typeAlias(string $name): ?string
    {
        return $this->typeAliases[$name] ?? null;
    }

    public function includeFor(string $type): ?string
    {
        $short = CPPName::type($type);
        $alias = $this->typeAlias($short);
        if ($alias === null || str_contains($alias, '<') || str_contains($alias, '::')) {
            return null;
        }
        if (in_array($alias, [
            'bool',
            'char',
            'int8_t',
            'uint8_t',
            'int16_t',
            'uint16_t',
            'int32_t',
            'uint32_t',
            'int64_t',
            'uint64_t',
            'float',
            'double',
            'std::string',
            'std::string_view',
        ], true)) {
            return null;
        }
        if (isset($this->typeIncludes[$short])) {
            return null;
        }
        return 'types/' . $alias . '.hpp';
    }

    /** @return list<string> */
    public function includesFor(string $type): array
    {
        return $this->typeIncludes[CPPName::type($type)] ?? [];
    }

    /** @return array<string, mixed>|null */
    public function typeCodec(string $name): ?array
    {
        return $this->typeCodecs[$name] ?? null;
    }

    public function isRuntimeDeclaration(string $name): bool
    {
        return isset($this->runtimeDeclarations[$name]);
    }

    public function isKnownValueType(string $name): bool
    {
        return isset($this->knownValueTypes[$name]);
    }

    public function isSerializerClass(string $name): bool
    {
        return isset($this->serializerClasses[$name]);
    }

    public function streamMethod(string $class, string $name): ?string
    {
        $normalized = strtolower($name);
        if (isset($this->streamMethods[$class][$normalized])) {
            return $this->streamMethods[$class][$normalized];
        }

        $className = strtolower($class);
        $reading = str_starts_with($normalized, 'get') || str_starts_with($normalized, 'read');
        $prefix = $reading ? 'read' : 'write';
        if ($className === 'varint') {
            $long = str_contains($normalized, 'long');
            $unsigned = str_contains($normalized, 'unsigned');
            return $prefix . ($unsigned ? 'Unsigned' : '') . ($long ? 'VarLong' : 'VarInt');
        }
        if ($className === 'le' || $className === 'be') {
            if (str_contains($normalized, 'bytearray')) {
                return $prefix . 'ByteArray';
            }
            if (str_contains($normalized, 'float')) {
                return $prefix . 'Float';
            }
            if (str_contains($normalized, 'unsignedlong')) {
                return $prefix . 'UnsignedLong';
            }
            if (str_contains($normalized, 'signedlong')) {
                return $prefix . 'Long';
            }
            if (str_contains($normalized, 'unsignedint')) {
                return $prefix . 'UnsignedInt';
            }
            if (str_contains($normalized, 'signedint')) {
                return $prefix . 'Int';
            }
            if (str_contains($normalized, 'unsignedshort')) {
                return $prefix . 'UnsignedShort';
            }
            if (str_contains($normalized, 'signedshort')) {
                return $prefix . 'Short';
            }
        }
        if (str_contains($normalized, 'bytearray')) {
            return $prefix . 'ByteArray';
        }
        if (str_contains($normalized, 'float')) {
            return $prefix . 'Float';
        }
        return null;
    }

    /** @return array<string, string>|null */
    public function specialCodec(string $class, string $name): ?array
    {
        return $this->specialCodecs[$class][strtolower($name)] ?? null;
    }

    public function constructor(string $name): ?string
    {
        return $this->constructors[$name] ?? null;
    }

    public function constantAlias(string $class, string $name): ?string
    {
        if ($class === 'PlayerAuthInputFlags' && $name !== 'NUMBER_OF_FLAGS') {
            return 'InputData::' . CPPName::enumCase($name);
        }
        return $this->constantAliases[$class][$name] ?? null;
    }

    public function getterAlias(string $class, string $name): ?string
    {
        return $this->getterAliases[$class][strtolower($name)] ?? null;
    }

    public function methodAlias(string $class, string $name): ?string
    {
        return $this->methodAliases[$class][strtolower($name)] ?? null;
    }

    public function staticCallAlias(string $class, string $name): ?string
    {
        return $this->staticCallAliases[$class][strtolower($name)] ?? null;
    }

    public function valueWriter(string $type, string $method): ?string
    {
        return $this->valueWriters[$type][strtolower($method)] ?? null;
    }

    /** @return array<string, mixed> */
    private static function defaults(): array
    {
        return CPPTranslationCatalogDefaults::values();
    }

    /** @param array<string, mixed> $config @return array<string, mixed> */
    private static function normalize(array $config): array
    {
        $keys = [
            'typeAliases',
            'typeIncludes',
            'typeCodecs',
            'runtimeDeclarations',
            'knownValueTypes',
            'serializerClasses',
            'streamMethods',
            'specialCodecs',
            'constructors',
            'constantAliases',
            'getterAliases',
            'methodAliases',
            'staticCallAliases',
            'valueWriters',
        ];
        $result = [];
        foreach ($keys as $key) {
            $value = $config[$key] ?? null;
            $result[$key] = match ($key) {
                'runtimeDeclarations', 'knownValueTypes', 'serializerClasses' => is_array($value) ? array_values(array_filter($value, 'is_string')) : [],
                default => is_array($value) ? $value : [],
            };
        }
        return $result;
    }
}
