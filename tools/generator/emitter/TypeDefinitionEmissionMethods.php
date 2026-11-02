<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\emitter;

use kodiak\bedrockprotocol\generator\Configuration;
use kodiak\bedrockprotocol\generator\CPPName;
use kodiak\bedrockprotocol\generator\CPP\CPPDefaultValueResolver;
use RuntimeException;

trait TypeDefinitionEmissionMethods
{
    private function writeTypeDefinitions(string $output, array $enums, array $classes): void
    {
        $base = $output . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock' . DIRECTORY_SEPARATOR . 'protocol' . DIRECTORY_SEPARATOR . 'types';
        $includes = [];
        foreach ($enums as $enum) {
            $includes[] = '#include <bedrock/protocol/' . $this->generatedTypeInclude($enum) . ">\n";
        }
        foreach ($classes as $class) {
            $includes[] = '#include <bedrock/protocol/' . $this->generatedTypeInclude($class) . ">\n";
        }

        $includeParts = [];
        foreach (array_chunk($includes, 200) as $index => $part) {
            $name = 'TypesGeneratedIncludes' . $index . '.hpp';
            $includeParts[] = $name;
            $this->write($base . DIRECTORY_SEPARATOR . $name, "#pragma once\n\n" . implode('', $part));
        }

        $definitions = '';
        foreach ($classes as $declaration) {
            $isAbstractType = ($declaration['abstract'] ?? false) || ($declaration['kind'] ?? null) === 'interface';
            if (!$isAbstractType) {
                continue;
            }
            $derivedTypes = $this->derivedTypes($declaration, $classes);
            if ($derivedTypes !== []) {
                $definitions .= $this->polymorphicFactoryDefinition($this->generatedTypeName($declaration), $derivedTypes);
            }
            if ($this->generatedTypeName($declaration) === 'GameRule') {
                $definitions .= $this->gameRuleDefinitions($derivedTypes);
            }
        }

        $factoryBody = "#pragma once\n\n";
        foreach ($includeParts as $includePart) {
            $factoryBody .= '#include <bedrock/protocol/types/' . $includePart . ">\n";
        }
        $factoryBody .= "\n" . $definitions;
        $this->write($base . DIRECTORY_SEPARATOR . 'TypesGeneratedFactories.hpp', $factoryBody);

        $body = "#pragma once\n\n";
        foreach ($includeParts as $includePart) {
            $body .= '#include <bedrock/protocol/types/' . $includePart . ">\n";
        }
        $body .= "#include <bedrock/protocol/types/TypesGeneratedFactories.hpp>\n";
        $this->write($base . DIRECTORY_SEPARATOR . 'TypesGenerated.hpp', $body);
    }

    private function gameRuleDefinitions(array $derivedTypes): string
    {
        $cases = [];
        foreach ($derivedTypes as $derivedType) {
            if (($derivedType['abstract'] ?? false) || ($derivedType['kind'] ?? null) === 'interface') {
                continue;
            }
            $id = $this->constantInteger($derivedType, 'ID');
            if ($id === null) {
                continue;
            }
            $decode = null;
            foreach ($derivedType['methods'] ?? [] as $method) {
                if (($method['static'] ?? false)
                    && strtolower((string) ($method['name'] ?? '')) === 'decode') {
                    $decode = $method;
                    break;
                }
            }
            if ($decode === null) {
                continue;
            }
            $arguments = ['stream', 'isPlayerModifiable'];
            if (count($decode['parameters'] ?? []) > 2) {
                $arguments[] = 'isStartGame';
            }
            $cases[] = '        case ' . $id . ': return ' . $this->generatedTypeName($derivedType)
                . '::create(' . implode(', ', $arguments) . ");\n";
        }
        $body = "\ninline std::shared_ptr<GameRule> GameRule::create(ReadOnlyBinaryStream& stream, const bool& isPlayerModifiable, const bool& isStartGame) {\n"
            . "    switch (stream.readUnsignedVarInt().value_or(0)) {\n"
            . implode('', $cases)
            . "        default: return nullptr;\n"
            . "    }\n}\n"
            . "\ninline std::map<std::string, std::shared_ptr<GameRule>> GameRule::readRules(ReadOnlyBinaryStream& stream, const bool& isStartGame) {\n"
            . "    std::map<std::string, std::shared_ptr<GameRule>> rules;\n"
            . "    const auto count = stream.readUnsignedVarInt().value_or(0);\n"
            . "    for (uint32_t index = 0; index < count; ++index) {\n"
            . "        auto name = stream.readString().value_or(\"\");\n"
            . "        const auto isPlayerModifiable = stream.readBool().value_or(false);\n"
            . "        rules.emplace(std::move(name), GameRule::create(stream, isPlayerModifiable, isStartGame));\n"
            . "    }\n"
            . "    return rules;\n}\n"
            . "\ninline void GameRule::writeRules(BinaryStream& stream, const std::map<std::string, std::shared_ptr<GameRule>>& rules, const bool& isStartGame) {\n"
            . "    stream.writeUnsignedVarInt(static_cast<uint32_t>(rules.size()));\n"
            . "    for (const auto& [name, rule] : rules) {\n"
            . "        stream.writeString(name);\n"
            . "        if (!rule) continue;\n"
            . "        stream.writeBool(rule->isPlayerModifiable);\n"
            . "        stream.writeUnsignedVarInt(static_cast<uint32_t>(rule->getTypeId()));\n"
            . "        rule->write(stream, isStartGame);\n"
            . "    }\n"
            . "}\n";
        return $body;
    }
}
