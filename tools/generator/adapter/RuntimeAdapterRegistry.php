<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\adapter;

use kodiak\bedrockprotocol\generator\CPPTranslationCatalog;

final class RuntimeAdapterRegistry extends CPPTranslationCatalog
{
    /** @var list<RuntimeAdapter> */
    private readonly array $adapters;

    /** @param array<string, mixed> $config */
    public function __construct(array $config = [])
    {
        parent::__construct($config);

        $catalog = new CPPTranslationCatalog($config);
        $this->adapters = [
            new TypeRuntimeAdapter($catalog),
            new StreamRuntimeAdapter($catalog),
            new SerializerRuntimeAdapter($catalog),
            new DeclarationRuntimeAdapter($catalog),
            new ValueRuntimeAdapter($catalog),
        ];
    }

    public function typeAlias(string $name): ?string
    {
        $value = $this->resolve('type.alias', ['name' => $name]);
        return is_string($value) ? $value : null;
    }

    public function includeFor(string $type): ?string
    {
        $value = $this->resolve('type.include', ['name' => $type]);
        return is_string($value) ? $value : null;
    }

    /** @return array<string, mixed>|null */
    public function typeCodec(string $name): ?array
    {
        $value = $this->resolve('type.codec', ['name' => $name]);
        return is_array($value) ? $value : null;
    }

    public function constructor(string $name): ?string
    {
        $value = $this->resolve('type.constructor', ['name' => $name]);
        return is_string($value) ? $value : null;
    }

    public function streamMethod(string $class, string $name): ?string
    {
        $value = $this->resolve('stream.method', ['class' => $class, 'name' => $name]);
        return is_string($value) ? $value : null;
    }

    /** @return array<string, string>|null */
    public function specialCodec(string $class, string $name): ?array
    {
        $value = $this->resolve('serializer.codec', ['class' => $class, 'name' => $name]);
        return is_array($value) ? $value : null;
    }

    public function isSerializerClass(string $name): bool
    {
        return (bool) $this->resolve('serializer.class', ['class' => $name]);
    }

    public function constantAlias(string $class, string $name): ?string
    {
        $value = $this->resolve('serializer.constant', ['class' => $class, 'name' => $name]);
        return is_string($value) ? $value : null;
    }

    public function getterAlias(string $class, string $name): ?string
    {
        $value = $this->resolve('serializer.getter', ['class' => $class, 'name' => $name]);
        return is_string($value) ? $value : null;
    }

    public function methodAlias(string $class, string $name): ?string
    {
        $value = $this->resolve('serializer.method', ['class' => $class, 'name' => $name]);
        return is_string($value) ? $value : null;
    }

    public function staticCallAlias(string $class, string $name): ?string
    {
        $value = $this->resolve('serializer.static', ['class' => $class, 'name' => $name]);
        return is_string($value) ? $value : null;
    }

    public function valueWriter(string $type, string $method): ?string
    {
        $value = $this->resolve('value.writer', ['type' => $type, 'method' => $method]);
        return is_string($value) ? $value : null;
    }

    public function isRuntimeDeclaration(string $name): bool
    {
        return (bool) $this->resolve('declaration.runtime', ['name' => $name]);
    }

    public function isKnownValueType(string $name): bool
    {
        return (bool) $this->resolve('declaration.value', ['name' => $name]);
    }

    /** @param array<string, mixed> $arguments */
    private function resolve(string $operation, array $arguments): mixed
    {
        foreach ($this->adapters as $adapter) {
            if ($adapter->supports($operation)) {
                return $adapter->adapt($operation, $arguments);
            }
        }

        return null;
    }
}
