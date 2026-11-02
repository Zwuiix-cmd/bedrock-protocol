<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\application;

use kodiak\bedrockprotocol\generator\adapter\RuntimeAdapterRegistry;
use kodiak\bedrockprotocol\generator\configuration\GeneratorConfiguration;
use kodiak\bedrockprotocol\generator\generation\ProtocolCodeGenerator;
use kodiak\bedrockprotocol\generator\parser\ProtocolSourceParser;
use kodiak\bedrockprotocol\generator\source\GitSourceRepository;
use kodiak\bedrockprotocol\generator\transform\ModelTransformer;
use kodiak\bedrockprotocol\generator\translation\PhpAstTransformer;
use Throwable;

final class GeneratorApplication
{
    public function __construct(
        private readonly GitSourceRepository $sourceRepository,
        private readonly ProtocolSourceParser $sourceParser,
        private readonly ModelTransformer $modelTransformer,
    ) {
    }

    /** @param list<string> $arguments */
    public function run(string $root, array $arguments): int
    {
        $source = null;

        try {
            $configuration = GeneratorConfiguration::fromArguments($root, $arguments);
            $source = $this->sourceRepository->resolve($configuration);
            $stats = ProtocolCodeGenerator::cachedStats($configuration, $source);

            if ($stats === null) {
                $model = $this->modelTransformer->transform($this->sourceParser->parse($source['root']));
                $adapters = new RuntimeAdapterRegistry($configuration->translation);
                $translator = new PhpAstTransformer($model['declarations'], $adapters);
                $stats = (new ProtocolCodeGenerator($translator, $adapters))->emit($model, $source, $configuration);
            }

            $this->printReport($stats, $source['revision']);
            return 0;
        } catch (Throwable $error) {
            fwrite(STDERR, 'generation failed: ' . $error->getMessage() . PHP_EOL);
            if (getenv('CODEGEN_TRACE') === '1') {
                fwrite(STDERR, $error->getTraceAsString() . PHP_EOL);
            }
            return 1;
        } finally {
            if (is_array($source)) {
                $this->sourceRepository->cleanup($source);
            }
        }
    }

    /** @param array{packets: int, declarations: int, enums: int, registrations: int, unsupported: int, changed: int, removed: int, cached: int} $stats */
    private function printReport(array $stats, string $revision): void
    {
        printf(
            "%s %d packets, %d declarations (%d enums), %d pool registrations from %s (%d files changed, %d removed)\n",
            $stats['cached'] === 1 ? 'cached' : 'generated',
            $stats['packets'],
            $stats['declarations'],
            $stats['enums'],
            $stats['registrations'],
            $revision,
            $stats['changed'],
            $stats['removed'],
        );
        if ($stats['unsupported'] > 0) {
            printf("adapter points recorded: %d (see metadata/codegen.json)\n", $stats['unsupported']);
        }
    }
}
