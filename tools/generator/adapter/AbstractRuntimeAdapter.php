<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\adapter;

use kodiak\bedrockprotocol\generator\CPPTranslationCatalog;

abstract class AbstractRuntimeAdapter implements RuntimeAdapter
{
    public function __construct(protected readonly CPPTranslationCatalog $catalog)
    {
    }

    /** @param list<string> $operations */
    protected function handles(string $operation, array $operations): bool
    {
        return in_array($operation, $operations, true);
    }
}
