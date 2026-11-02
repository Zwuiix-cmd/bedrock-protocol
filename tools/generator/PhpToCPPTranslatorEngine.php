<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator;

use kodiak\bedrockprotocol\generator\translation\AnalysisTranslationMethods;
use kodiak\bedrockprotocol\generator\translation\BodyTranslationMethods;
use kodiak\bedrockprotocol\generator\translation\CallTranslationMethods;
use kodiak\bedrockprotocol\generator\translation\EnumTranslationMethods;
use kodiak\bedrockprotocol\generator\translation\ExpressionTranslationMethods;
use kodiak\bedrockprotocol\generator\translation\FormattingTranslationMethods;
use kodiak\bedrockprotocol\generator\translation\InferenceTranslationMethods;
use kodiak\bedrockprotocol\generator\translation\LoopTranslationMethods;
use kodiak\bedrockprotocol\generator\translation\PhpInteropTranslationMethods;
use kodiak\bedrockprotocol\generator\translation\PropertyCodecInferenceMethods;
use kodiak\bedrockprotocol\generator\translation\StatementTranslationMethods;
use kodiak\bedrockprotocol\generator\translation\StringEnumTranslationMethods;
use kodiak\bedrockprotocol\generator\translation\TypeTranslationMethods;
use kodiak\bedrockprotocol\generator\translation\ValueCodecTranslationMethods;

/**
 * Stateful implementation composed from focused translation components.
 *
 * The public API is exposed by PhpToCPPTranslator; this object owns the
 * shared inference state used while translating one protocol model.
 */
final class PhpToCPPTranslatorEngine
{
    use AnalysisTranslationMethods;
    use BodyTranslationMethods;
    use CallTranslationMethods;
    use EnumTranslationMethods;
    use ExpressionTranslationMethods;
    use FormattingTranslationMethods;
    use InferenceTranslationMethods;
    use LoopTranslationMethods;
    use PhpInteropTranslationMethods;
    use PropertyCodecInferenceMethods;
    use StatementTranslationMethods;
    use StringEnumTranslationMethods;
    use TypeTranslationMethods;
    use ValueCodecTranslationMethods;

    /** @var array<string, array<string, mixed>> */
    private array $declarations;

    private CPPTranslationCatalog $catalog;

    /** @var list<array{class: string, method: string, line: int, reason: string}> */
    private array $unsupported = [];

    /** @var array<string, string> */
    private array $expressionTypes = [];

    /** @var array<string, string> */
    private array $expressionAliases = [];

    /** @var array<string, bool> */
    private array $expressionOptionals = [];

    /** @var array<string, true> */
    private array $localVariables = [];

    /** @var array<string, true> */
    private array $declaredVariables = [];

    /** @var array<string, array<string, string>> */
    private array $inferredPropertyTypes = [];

    /** @var array<string, array<string, array{read: string, write: string}>> */
    private array $inferredPropertyCodecs = [];

    /** @var array<string, list<string>> */
    private array $inferredPropertyOrder = [];

    /** @var list<array<string, string>> */
    private array $arrayScopes = [];

    /** @var array<string, true> */
    private array $inlinedMethods = [];

    /** @param array<string, array<string, mixed>> $declarations */
    public function __construct(array $declarations, ?CPPTranslationCatalog $catalog = null)
    {
        $this->declarations = $declarations;
        $this->catalog = $catalog ?? new CPPTranslationCatalog();
        $this->inferPropertyCodecs();
    }

    /** @return list<array{class: string, method: string, line: int, reason: string}> */
    public function unsupported(): array
    {
        return $this->unsupported;
    }
}
