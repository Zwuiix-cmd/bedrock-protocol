<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\transform;

final class ModelTransformationPipeline extends ModelTransformer
{
    /** @param list<ModelTransformer> $transformers */
    public function __construct(private readonly array $transformers)
    {
    }

    /** @param array<string, mixed> $model @return array<string, mixed> */
    protected function apply(array $model): array
    {
        foreach ($this->transformers as $transformer) {
            $model = $transformer->transform($model);
        }

        return $model;
    }
}
