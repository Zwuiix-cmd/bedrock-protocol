<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\configuration;

use kodiak\bedrockprotocol\generator\Configuration;

final class GeneratorConfiguration extends Configuration
{
    /** @param list<string> $arguments */
    public static function fromArguments(string $root, array $arguments): self
    {
        $configuration = parent::fromArguments($root, $arguments);

        return new self(
            $configuration->sourceUrl,
            $configuration->sourceRef,
            $configuration->outputDirectory,
            $configuration->configPath,
            $configuration->translation,
        );
    }
}
