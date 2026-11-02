<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\adapter;

final class TypeRuntimeAdapter extends AbstractRuntimeAdapter
{
    public function supports(string $operation): bool
    {
        return $this->handles($operation, ['type.alias', 'type.codec', 'type.include', 'type.constructor']);
    }

    public function adapt(string $operation, array $arguments): mixed
    {
        return match ($operation) {
            'type.alias' => $this->catalog->typeAlias((string) ($arguments['name'] ?? '')),
            'type.codec' => $this->catalog->typeCodec((string) ($arguments['name'] ?? '')),
            'type.include' => $this->catalog->includeFor((string) ($arguments['name'] ?? '')),
            'type.constructor' => $this->catalog->constructor((string) ($arguments['name'] ?? '')),
            default => null,
        };
    }
}
