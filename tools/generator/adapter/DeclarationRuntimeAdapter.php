<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\adapter;

final class DeclarationRuntimeAdapter extends AbstractRuntimeAdapter
{
    public function supports(string $operation): bool
    {
        return $this->handles($operation, ['declaration.runtime', 'declaration.value']);
    }

    public function adapt(string $operation, array $arguments): mixed
    {
        $name = (string) ($arguments['name'] ?? '');

        return match ($operation) {
            'declaration.runtime' => $this->catalog->isRuntimeDeclaration($name),
            'declaration.value' => $this->catalog->isKnownValueType($name),
            default => null,
        };
    }
}
