<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\adapter;

final class SerializerRuntimeAdapter extends AbstractRuntimeAdapter
{
    public function supports(string $operation): bool
    {
        return $this->handles($operation, [
            'serializer.class',
            'serializer.codec',
            'serializer.constant',
            'serializer.getter',
            'serializer.method',
            'serializer.static',
        ]);
    }

    public function adapt(string $operation, array $arguments): mixed
    {
        $class = (string) ($arguments['class'] ?? '');
        $name = (string) ($arguments['name'] ?? '');

        return match ($operation) {
            'serializer.class' => $this->catalog->isSerializerClass($class),
            'serializer.codec' => $this->catalog->specialCodec($class, $name),
            'serializer.constant' => $this->catalog->constantAlias($class, $name),
            'serializer.getter' => $this->catalog->getterAlias($class, $name),
            'serializer.method' => $this->catalog->methodAlias($class, $name),
            'serializer.static' => $this->catalog->staticCallAlias($class, $name),
            default => null,
        };
    }
}
