<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\adapter;

final class ValueRuntimeAdapter extends AbstractRuntimeAdapter
{
    public function supports(string $operation): bool
    {
        return $operation === 'value.writer';
    }

    public function adapt(string $operation, array $arguments): mixed
    {
        if (!$this->supports($operation)) {
            return null;
        }

        return $this->catalog->valueWriter(
            (string) ($arguments['type'] ?? ''),
            (string) ($arguments['method'] ?? ''),
        );
    }
}
