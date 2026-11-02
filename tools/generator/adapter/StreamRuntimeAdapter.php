<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\adapter;

final class StreamRuntimeAdapter extends AbstractRuntimeAdapter
{
    public function supports(string $operation): bool
    {
        return $operation === 'stream.method';
    }

    public function adapt(string $operation, array $arguments): mixed
    {
        if (!$this->supports($operation)) {
            return null;
        }

        return $this->catalog->streamMethod(
            (string) ($arguments['class'] ?? ''),
            (string) ($arguments['name'] ?? ''),
        );
    }
}
