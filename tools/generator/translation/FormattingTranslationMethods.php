<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\translation;

use PhpParser\Node;
use kodiak\bedrockprotocol\generator\CPPName;

trait FormattingTranslationMethods
{
private function indentLines(array $lines, int $indent): string
    {
        $prefix = str_repeat("\t", $indent);
        return implode("\n", array_map(static fn (string $line): string => $prefix . $line, $lines));
    }

private function callback(string $parameters, string $body): string
    {
        $body = trim($body);
        $lines = array_map(
            static fn (string $line): string => "\t" . ltrim($line),
            explode("\n", $body),
        );
        return '[&](' . $parameters . ") {\n" . implode("\n", $lines) . "\n}";
    }

private function escape(string $value): string
    {
        return addcslashes($value, "\\\"");
    }
}
