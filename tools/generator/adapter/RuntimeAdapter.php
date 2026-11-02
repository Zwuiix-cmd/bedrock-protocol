<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\adapter;

interface RuntimeAdapter
{
    public function supports(string $operation): bool;

    /** @param array<string, mixed> $arguments */
    public function adapt(string $operation, array $arguments): mixed;
}
