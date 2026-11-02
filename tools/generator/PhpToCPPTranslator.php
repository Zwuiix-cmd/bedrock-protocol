<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator;

use PhpParser\Node;

/**
 * Public translator facade used by the emitter.
 *
 * Translation responsibilities live in the composed engine components under
 * translation/. Keeping this API stable lets the emitter remain independent
 * from the internal translation pipeline.
 */
class PhpToCPPTranslator
{
    private PhpToCPPTranslatorEngine $engine;

    /** @param array<string, array<string, mixed>> $declarations */
    public function __construct(array $declarations, ?CPPTranslationCatalog $catalog = null)
    {
        $this->engine = new PhpToCPPTranslatorEngine($declarations, $catalog);
    }

    /** @return list<array{class: string, method: string, line: int, reason: string}> */
    public function unsupported(): array
    {
        return $this->engine->unsupported();
    }

    /** @param array<string, mixed> $method @param array<string, mixed> $declaration */
    public function methodBody(array $method, array $declaration, bool $returnShared = false): string
    {
        return $this->engine->methodBody($method, $declaration, $returnShared);
    }

    /** @param array<string, mixed> $method @param array<string, mixed> $declaration @param array<string, string> $parameterAliases */
    public function methodBodyWithAliases(array $method, array $declaration, array $parameterAliases): string
    {
        return $this->engine->methodBodyWithAliases($method, $declaration, $parameterAliases);
    }

    /** @param array<string, mixed> $declaration */
    public function genericBody(array $declaration, bool $read, bool $returnShared = false): string
    {
        return $this->engine->genericBody($declaration, $read, $returnShared);
    }

    public function expression(?Node $expression, array $declaration, ?array $method = null, ?string $expectedType = null): string
    {
        return $this->engine->expression($expression, $declaration, $method, $expectedType);
    }

    public function cppType(?string $type, bool $stringView = false, ?string $doc = null): string
    {
        return $this->engine->cppType($type, $stringView, $doc);
    }

    /** @param array<string, mixed> $property @param array<string, mixed> $declaration */
    public function cppPropertyType(array $property, array $declaration): string
    {
        return $this->engine->cppPropertyType($property, $declaration);
    }
}
