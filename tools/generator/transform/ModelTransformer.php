<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\transform;

abstract class ModelTransformer
{
    /** @param array<string, mixed> $model @return array<string, mixed> */
    final public function transform(array $model): array
    {
        return $this->apply($model);
    }

    /** @param array<string, mixed> $model @return array<string, mixed> */
    abstract protected function apply(array $model): array;
}
