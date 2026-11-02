<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\emitter;

use kodiak\bedrockprotocol\generator\CPPName;
use kodiak\bedrockprotocol\generator\CPP\CPPDefaultValueResolver;

trait PlayerBlockActionEmissionMethods
{
    /** @param list<string> $validActionExpressions */
    private function playerBlockActionUnifiedHeader(
        array $declaration,
        array $validActionExpressions,
        string $playerActionInclude,
    ): string {
        $properties = array_values(array_filter(
            $declaration['properties'] ?? [],
            static fn (array $property): bool => !($property['static'] ?? false),
        ));
        $body = <<<'CPP'
#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <bedrock/protocol/@PLAYER_ACTION_INCLUDE@>

struct PlayerBlockAction {
CPP;
        $body .= "\n";

        foreach ($properties as $property) {
            $name = CPPName::identifier((string) $property['name']);
            $type = $this->runtimePropertyType($property);
            $body .= '    ' . $type . ' ' . $name;
            $default = CPPDefaultValueResolver::member($type);
            $body .= $default === null ? ";\n" : ' = ' . $default . ";\n";
        }

        if ($properties !== []) {
            $parameters = [];
            $initializers = [];
            foreach ($properties as $property) {
                $name = CPPName::identifier((string) $property['name']);
                $parameters[] = 'const ' . $this->runtimePropertyType($property) . '& ' . $name . 'Value';
                $initializers[] = $name . '(' . $name . 'Value)';
            }
            $body .= "\n    explicit PlayerBlockAction(\n        "
                . implode(",\n        ", $parameters)
                . "\n    )\n        : "
                . implode(",\n          ", $initializers)
                . "\n    {}\n";
        }

        $body .= "\n    void write(BinaryStream& stream) const {\n";
        foreach ($properties as $property) {
            $name = CPPName::identifier((string) $property['name']);
            $body .= '        ' . $this->runtimeWriteExpression($property, $name, true) . "\n";
        }
        $body .= "    }\n\n    void read(ReadOnlyBinaryStream& stream) {\n";
        foreach ($properties as $property) {
            $name = CPPName::identifier((string) $property['name']);
            $body .= '        ' . $name . ' = ' . $this->runtimeReadExpression($property, true) . ";\n";
        }
        $body .= "    }\n\n    static bool isValidActionType(int32_t value) noexcept {\n";
        $checks = array_map(
            static fn (string $expression): string => 'value == static_cast<int32_t>(' . $expression . ')',
            $validActionExpressions,
        );
        if ($checks === []) {
            $body .= "        return true;\n";
        } else {
            $body .= "        return\n            " . implode("\n            || ", $checks) . ";\n";
        }
        $body .= "    }\n\n";
        $body .= <<<'CPP'
    static PlayerBlockAction decodeWithBlockInfo(ReadOnlyBinaryStream& stream, int32_t value) {
        PlayerBlockAction result;
        result.actionType = value;
        result.read(stream);
        return result;
    }
};
CPP;
        return str_replace('@PLAYER_ACTION_INCLUDE@', $playerActionInclude, $body);
    }
}
