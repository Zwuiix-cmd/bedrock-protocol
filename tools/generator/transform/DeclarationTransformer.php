<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\transform;

final class DeclarationTransformer extends ModelTransformer
{
    /** @param array<string, mixed> $model @return array<string, mixed> */
    protected function apply(array $model): array
    {
        $declarations = [];
        foreach ($model['declarations'] ?? [] as $key => $declaration) {
            if (!is_array($declaration)) {
                continue;
            }

            $declaration['constants'] = $this->list($declaration['constants'] ?? []);
            $declaration['cases'] = $this->list($declaration['cases'] ?? []);
            $declaration['properties'] = $this->list($declaration['properties'] ?? []);
            $declaration['methods'] = $this->list($declaration['methods'] ?? []);
            $declaration['implements'] = $this->list($declaration['implements'] ?? []);
            $declaration['parent'] = is_string($declaration['parent'] ?? null) ? $declaration['parent'] : null;
            $declaration['abstract'] = (bool) ($declaration['abstract'] ?? false);
            $declaration['final'] = (bool) ($declaration['final'] ?? false);

            $declarations[(string) $key] = $declaration;
        }

        $model['declarations'] = $declarations;
        return $model;
    }

    /** @param mixed $value @return list<mixed> */
    private function list(mixed $value): array
    {
        return is_array($value) ? array_values($value) : [];
    }
}
