<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator;

use PhpParser\Node;
use PhpParser\NodeTraverser;
use PhpParser\NodeVisitor\NameResolver;
use PhpParser\ParserFactory;
use PhpParser\PrettyPrinter\Standard;
use kodiak\bedrockprotocol\generator\parser\ConstantResolutionMethods;
use kodiak\bedrockprotocol\generator\parser\DeclarationParsingMethods;
use RuntimeException;

/**
 * Parses the upstream PHP model and delegates declaration/value concerns to
 * focused parser components.
 */
class PhpProtocolParser
{
    use ConstantResolutionMethods;
    use DeclarationParsingMethods;

    private readonly \PhpParser\Parser $parser;
    private readonly Standard $printer;

    /** @var array<string, array<string, mixed>> */
    private array $declarations = [];

    /** @var array<string, array<string, mixed>> */
    private array $constants = [];

    public function __construct()
    {
        $this->parser = (new ParserFactory())->createForNewestSupportedVersion();
        $this->printer = new Standard();
    }

    /**
     * @return array{
     *   declarations: array<string, array<string, mixed>>,
     *   packets: array<int, array<string, mixed>>,
     *   enums: array<int, array<string, mixed>>,
     *   protocolConstants: array<string, int|string|float|bool|null>,
     *   registrations: array<int, string>,
     *   files: int,
     *   fileHashes: array<string, string>
     * }
     */
    public function parse(string $sourceRoot): array
    {
        $this->declarations = [];
        $this->constants = [];
        $files = $this->phpFiles($sourceRoot . DIRECTORY_SEPARATOR . 'src');
        $fileHashes = [];
        foreach ($files as $file) {
            $code = file_get_contents($file);
            if ($code === false) {
                throw new RuntimeException('Unable to read ' . $file);
            }
            $relative = ltrim(str_replace($sourceRoot, '', $file), DIRECTORY_SEPARATOR);
            $relative = str_replace('\\', '/', $relative);
            $hash = hash_file('sha256', $file);
            if ($hash === false) {
                throw new RuntimeException('Unable to hash ' . $file);
            }
            $fileHashes[$relative] = $hash;
            try {
                $ast = $this->parser->parse($code);
            } catch (\Throwable $error) {
                throw new RuntimeException('Unable to parse ' . $file . ': ' . $error->getMessage(), 0, $error);
            }
            if ($ast === null) {
                continue;
            }
            $traverser = new NodeTraverser();
            $traverser->addVisitor(new NameResolver());
            $ast = $traverser->traverse($ast);
            $this->collectDeclarations($ast, $sourceRoot, $file);
        }

        $this->resolveConstantValues();
        $declarations = array_values($this->declarations);
        usort($declarations, static fn (array $left, array $right): int => [$left['kind'], $left['name']] <=> [$right['kind'], $right['name']]);

        $packets = array_values(array_filter($declarations, fn (array $declaration): bool => $this->isPacket($declaration)));
        usort($packets, static fn (array $left, array $right): int => ((int) ($left['networkId'] ?? PHP_INT_MAX)) <=> ((int) ($right['networkId'] ?? PHP_INT_MAX)));

        return [
            'declarations' => array_combine(array_map(static fn (array $item): string => $item['fqcn'], $declarations), $declarations) ?: [],
            'packets' => $packets,
            'enums' => array_values(array_filter($declarations, static fn (array $declaration): bool => $declaration['kind'] === 'enum')),
            'protocolConstants' => $this->protocolConstants(),
            'registrations' => $this->findPoolRegistrations(),
            'files' => count($files),
            'fileHashes' => $fileHashes,
        ];
    }
}
