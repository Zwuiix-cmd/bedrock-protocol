<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator;

use kodiak\bedrockprotocol\generator\emitter\DefaultValueEmissionMethods;
use kodiak\bedrockprotocol\generator\emitter\DeclarationEmissionMethods;
use kodiak\bedrockprotocol\generator\emitter\LifecycleEmissionMethods;
use kodiak\bedrockprotocol\generator\emitter\ManifestEmissionMethods;
use kodiak\bedrockprotocol\generator\emitter\MetadataPropertyEmissionMethods;
use kodiak\bedrockprotocol\generator\emitter\MetadataTypesEmissionMethods;
use kodiak\bedrockprotocol\generator\emitter\PacketEmissionMethods;
use kodiak\bedrockprotocol\generator\emitter\PathEmissionMethods;
use kodiak\bedrockprotocol\generator\emitter\PlayerBlockActionEmissionMethods;
use kodiak\bedrockprotocol\generator\emitter\PoolAndEnumEmissionMethods;
use kodiak\bedrockprotocol\generator\emitter\RuntimeSupportEmissionMethods;
use kodiak\bedrockprotocol\generator\emitter\TypeDefinitionEmissionMethods;
use kodiak\bedrockprotocol\generator\emitter\TypeEmissionMethods;

/**
 * C++ protocol emitter facade composed from focused output components.
 */
class CPPEmitter
{
    use DefaultValueEmissionMethods;
    use DeclarationEmissionMethods;
    use LifecycleEmissionMethods;
    use ManifestEmissionMethods;
    use MetadataPropertyEmissionMethods;
    use MetadataTypesEmissionMethods;
    use PacketEmissionMethods;
    use PathEmissionMethods;
    use PlayerBlockActionEmissionMethods;
    use PoolAndEnumEmissionMethods;
    use RuntimeSupportEmissionMethods;
    use TypeDefinitionEmissionMethods;
    use TypeEmissionMethods;

    private string $outputRoot = '';

    /** @var array<string, true> */
    private array $emittedFiles = [];

    /** @var array<string, true> */
    private array $changedFiles = [];

    /** @var array<string, mixed> */
    private array $previousManifest = [];

    /** @var array<string, true> */
    private array $enumNames = [];

    /** @var array<string, string> Source and generated type names to include paths. */
    private array $typeIncludePaths = [];

    private function protocolNamespaceOpen(): string
    {
        return "namespace bedrock::protocol {\n\n";
    }

    private function protocolNamespaceClose(): string
    {
        return "\n} // namespace bedrock::protocol\n";
    }

    private function qualifiedProtocolType(string $type): string
    {
        return '::bedrock::protocol::' . ltrim($type, ':');
    }

    private function wrapProtocolHeader(string $body): string
    {
        $lines = explode("\n", $body);
        $declarationIndex = null;

        foreach ($lines as $index => $line) {
            $trimmed = trim($line);
            if ($trimmed === ''
                || str_starts_with($trimmed, '#')
                || str_starts_with($trimmed, '//')) {
                continue;
            }
            $declarationIndex = $index;
            break;
        }

        if ($declarationIndex === null) {
            return $body;
        }

        return implode(
            "\n",
            array_merge(
                array_slice($lines, 0, $declarationIndex),
                [$this->protocolNamespaceOpen()],
                array_slice($lines, $declarationIndex),
                [$this->protocolNamespaceClose()],
            ),
        );
    }

    public function __construct(
        private readonly PhpToCPPTranslator $translator,
        private readonly CPPTranslationCatalog $catalog = new CPPTranslationCatalog(),
    ) {
    }
}
