<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\emitter;

use kodiak\bedrockprotocol\generator\Configuration;
use kodiak\bedrockprotocol\generator\CPPName;
use kodiak\bedrockprotocol\generator\CPP\CPPDefaultValueResolver;
use RuntimeException;

trait MetadataTypesEmissionMethods
{
    private function playerAuthInputFlagsHeader(array $declaration): string
    {
        $constants = [];
        $count = null;
        foreach ($declaration['constants'] ?? [] as $constant) {
            $name = (string) ($constant['name'] ?? '');
            $value = $constant['value'] ?? null;
            if ($name === 'NUMBER_OF_FLAGS') {
                $count = is_int($value) ? $value : null;
                continue;
            }
            if ($name !== '' && is_int($value)) {
                $constants[] = [$name, $value];
            }
        }
        if ($count === null) {
            $count = count($constants);
        }

        $body = <<<'CPP'
#pragma once

#include <bitset>
#include <cstddef>
#include <cstdint>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

enum class InputData : int {
CPP;
        $body .= "\n";
        foreach ($constants as [$name, $value]) {
            $body .= '    ' . CPPName::identifier($name) . ' = ' . $value . ",\n";
        }
        $body .= <<<'CPP'
};

struct PlayerAuthInputFlags {
    static constexpr std::size_t Count =
CPP;
        $body .= '        ' . $count . ";\n\n";
        $body .= <<<'CPP'
    std::bitset<Count> flags;

    PlayerAuthInputFlags() = default;
    explicit PlayerAuthInputFlags(std::size_t) {}

    [[nodiscard]] bool hasFlag(InputData flag) const noexcept {
        return flags.test(static_cast<std::size_t>(flag));
    }

    [[nodiscard]] bool get(std::size_t flag) const noexcept {
        return flag < Count && flags.test(flag);
    }

    [[nodiscard]] bool get(InputData flag) const noexcept {
        return get(static_cast<std::size_t>(flag));
    }

    [[nodiscard]] std::size_t getLength() const noexcept {
        return Count;
    }

    void setFlag(InputData flag, bool state) noexcept {
        flags.set(static_cast<std::size_t>(flag), state);
    }

    void set(std::size_t flag, bool state) noexcept {
        if (flag < Count) {
            flags.set(flag, state);
        }
    }

    void addFlag(InputData flag) noexcept {
        setFlag(flag, true);
    }

    void removeFlag(InputData flag) noexcept {
        setFlag(flag, false);
    }

    void write(BinaryStream& stream) const {
        for (std::size_t offset = 0; offset < Count; offset += 7) {
            uint8_t byte = 0;
            for (std::size_t bit = 0; bit < 7 && offset + bit < Count; ++bit) {
                if (flags.test(offset + bit)) {
                    byte |= static_cast<uint8_t>(uint8_t{1} << bit);
                }
            }
            if (offset + 7 < Count) {
                byte |= uint8_t{0x80};
            }
            stream.writeByte(static_cast<int8_t>(byte));
        }
    }

    void read(ReadOnlyBinaryStream& stream) {
        flags.reset();
        for (std::size_t offset = 0; offset < Count; offset += 7) {
            const uint8_t byte = static_cast<uint8_t>(stream.readByte().value_or(0));
            const uint8_t payload = static_cast<uint8_t>(byte & uint8_t{0x7f});
            for (std::size_t bit = 0; bit < 7 && offset + bit < Count; ++bit) {
                if ((payload & static_cast<uint8_t>(uint8_t{1} << bit)) != 0) {
                    flags.set(offset + bit);
                }
            }
            if ((byte & uint8_t{0x80}) == 0) {
                break;
            }
        }
    }
};
CPP;
        return $body;
    }

    private function playerAuthInputVehicleInfoHeader(array $declaration): string
    {
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

struct PlayerAuthInputVehicleInfo {
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
            $body .= "\n    PlayerAuthInputVehicleInfo() = default;\n\n    explicit PlayerAuthInputVehicleInfo(";
            $parameters = [];
            $initializers = [];
            foreach ($properties as $property) {
                $name = CPPName::identifier((string) $property['name']);
                $parameters[] = 'const ' . $this->runtimePropertyType($property) . '& ' . $name . 'Value';
                $initializers[] = $name . '(' . $name . 'Value)';
            }
            $body .= implode(', ', $parameters) . ') : ' . implode(', ', $initializers) . " {}\n";
        }
        $body .= "\n    void write(BinaryStream& stream) const {\n";
        foreach ($properties as $property) {
            $body .= '        ' . $this->runtimeWriteExpression($property, CPPName::identifier((string) $property['name'])) . "\n";
        }
        $body .= "    }\n\n    void read(ReadOnlyBinaryStream& stream) {\n";
        foreach ($properties as $property) {
            $name = CPPName::identifier((string) $property['name']);
            $body .= '        ' . $name . ' = ' . $this->runtimeReadExpression($property) . ";\n";
        }
        $body .= "    }\n};\n";
        return $body;
    }

    private function playerBlockActionHeader(
        array $declaration,
        ?array $withBlockInfo,
        ?array $stopBreak,
        string $playerActionInclude,
    ): string
    {
        $validationDeclaration = $withBlockInfo ?? $declaration;
        $validActionExpressions = [];
        foreach ($validationDeclaration['methods'] ?? [] as $method) {
            if (($method['name'] ?? '') !== 'isValidActionType') {
                continue;
            }
            foreach ($method['body'] ?? [] as $statement) {
                if (!$statement instanceof \PhpParser\Node\Stmt\Return_ || !$statement->expr instanceof \PhpParser\Node\Expr\Match_) {
                    continue;
                }
                foreach ($statement->expr->arms as $arm) {
                    if (!$arm->body instanceof \PhpParser\Node\Expr\ConstFetch
                        || strtolower($arm->body->name->toString()) !== 'true') {
                        continue;
                    }
                    foreach ($arm->conds ?? [] as $condition) {
                        $validActionExpressions[] = $this->translator->expression($condition, $validationDeclaration, $method);
                    }
                }
            }
        }
        $validActionExpressions = array_values(array_unique($validActionExpressions));

        if ($withBlockInfo === null && $stopBreak === null) {
            return $this->playerBlockActionUnifiedHeader(
                $declaration,
                $validActionExpressions,
                $playerActionInclude,
            );
        }

        if ($validActionExpressions === []) {
            throw new RuntimeException('PlayerBlockActionWithBlockInfo::isValidActionType has no true match arms');
        }

        $stopActionExpression = null;
        foreach ($stopBreak['methods'] ?? [] as $method) {
            if (($method['name'] ?? '') !== 'getActionType') {
                continue;
            }
            foreach ($method['body'] ?? [] as $statement) {
                if ($statement instanceof \PhpParser\Node\Stmt\Return_ && $statement->expr !== null) {
                    $stopActionExpression = $this->translator->expression($statement->expr, $stopBreak, $method);
                    break 2;
                }
            }
        }
        if ($stopActionExpression === null) {
            throw new RuntimeException('PlayerBlockActionStopBreak::getActionType has no return expression');
        }
        $stopActionExpression = 'static_cast<int32_t>(' . $stopActionExpression . ')';
        $validActionExpressions = array_map(
            static fn (string $expression): string => 'static_cast<int32_t>(' . $expression . ')',
            $validActionExpressions,
        );
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
        $body .= "\n    int32_t actionType = " . $stopActionExpression . ";\n    glm::ivec3 blockPosition = glm::ivec3(0);\n    int32_t face = 0;\n\n";
        foreach ($properties as $property) {
            $name = CPPName::identifier((string) $property['name']);
            $type = $this->runtimePropertyType($property);
            $body .= '    ' . $type . ' ' . $name;
            $default = CPPDefaultValueResolver::member($type);
            $body .= $default === null ? ";\n" : ' = ' . $default . ";\n";
        }
        if ($properties !== []) {
            $body .= "\n    explicit PlayerBlockAction(";
            $parameters = [];
            $initializers = [];
            foreach ($properties as $property) {
                $name = CPPName::identifier((string) $property['name']);
                $parameters[] = 'const ' . $this->runtimePropertyType($property) . '& ' . $name . 'Value';
                $initializers[] = $name . '(' . $name . 'Value)';
            }
            $body .= implode(', ', $parameters) . ') : ' . implode(', ', $initializers) . " {}\n";
        }
        $body .= "\n    void write(BinaryStream& stream) const {\n";
        foreach ($properties as $property) {
            $body .= '        ' . $this->runtimeWriteExpression($property, CPPName::identifier((string) $property['name']), true) . "\n";
        }
        $body .= "        if (actionType != " . $stopActionExpression . " && isValidActionType(actionType)) {\n            stream.writeSignedBlockPos(blockPosition);\n            stream.writeVarInt(face);\n        }\n";
        $body .= "    }\n\n    void read(ReadOnlyBinaryStream& stream) {\n";
        foreach ($properties as $property) {
            $name = CPPName::identifier((string) $property['name']);
            $body .= '        ' . $name . ' = ' . $this->runtimeReadExpression($property, true) . ";\n";
        }
        $body .= "        if (actionType != " . $stopActionExpression . " && isValidActionType(actionType)) {\n            blockPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));\n            face = stream.readVarInt().value_or(0);\n        }\n";
        $body .= "    }\n\n    static bool isValidActionType(int32_t value) noexcept {\n";
        $body .= '        return ' . implode(' || ', array_map(static fn (string $value): string => 'value == ' . $value, $validActionExpressions)) . ";\n    }\n\n";
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

    private function networkItemDescriptorHeader(array $declaration): string
    {
        $fields = [];
        $properties = array_values(array_filter(
            $declaration['properties'] ?? [],
            static fn (array $property): bool => !($property['static'] ?? false),
        ));
        foreach ($properties as $property) {
            $fields[strtolower((string) $property['name'])] = CPPName::identifier((string) $property['name']);
        }
        foreach (['id', 'count', 'meta', 'blockruntimeid', 'rawextradata'] as $field) {
            if (!isset($fields[$field])) {
                throw new RuntimeException('ItemStack source field is missing: ' . $field);
            }
        }
        $id = $fields['id'];
        $count = $fields['count'];
        $meta = $fields['meta'];
        $blockRuntimeId = $fields['blockruntimeid'];
        $rawExtraData = $fields['rawextradata'];
        $body = <<<'CPP'
#pragma once

#include <cstdint>
#include <string>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

struct NetworkItemDescriptor {
CPP;
        $body .= "\n";
        foreach ($properties as $property) {
            $name = CPPName::identifier((string) $property['name']);
            $type = $this->networkItemPropertyType($property);
            $body .= '    ' . $type . ' ' . $name;
            $default = CPPDefaultValueResolver::member($type);
            $body .= $default === null ? ";\n" : ' = ' . $default . ";\n";
        }
        $body .= "\n    void write(BinaryStream& stream) const {\n";
        $body .= '        stream.writeShort(static_cast<int16_t>(' . $id . "));\n";
        $body .= '        stream.writeUnsignedShort(' . $count . ");\n";
        $body .= '        stream.writeUnsignedVarInt(' . $meta . ");\n";
        $body .= '        stream.writeUnsignedVarInt(static_cast<uint32_t>(' . $blockRuntimeId . "));\n";
        $body .= '        stream.writeString(' . $rawExtraData . ");\n    }\n\n";
        $body .= "    void read(ReadOnlyBinaryStream& stream) {\n";
        $body .= '        ' . $id . " = stream.readShort().value_or(0);\n";
        $body .= '        ' . $count . " = stream.readUnsignedShort().value_or(0);\n";
        $body .= '        ' . $meta . " = stream.readUnsignedVarInt().value_or(0);\n";
        $body .= '        ' . $blockRuntimeId . " = stream.readUnsignedVarInt().value_or(0);\n";
        $body .= '        if (!stream.readString(' . $rawExtraData . ")) {\n            " . $rawExtraData . ".clear();\n        }\n    }\n";
        return $body . "};\n";
    }

    private function networkItemStackHeader(array $itemStack, array $wrapper, string $descriptorInclude): string
    {
        $fields = [];
        foreach ($itemStack['properties'] ?? [] as $property) {
            if (!($property['static'] ?? false)) {
                $fields[strtolower((string) $property['name'])] = CPPName::identifier((string) $property['name']);
            }
        }
        foreach (['id', 'count', 'meta', 'blockruntimeid', 'rawextradata'] as $field) {
            if (!isset($fields[$field])) {
                throw new RuntimeException('ItemStack source field is missing: ' . $field);
            }
        }

        $stackId = null;
        foreach ($wrapper['properties'] ?? [] as $property) {
            if (strtolower((string) $property['name']) === 'stackid') {
                $stackId = CPPName::identifier((string) $property['name']);
                break;
            }
        }
        if ($stackId === null) {
            throw new RuntimeException('ItemStackWrapper source field is missing: stackId');
        }

        $id = $fields['id'];
        $count = $fields['count'];
        $meta = $fields['meta'];
        $blockRuntimeId = $fields['blockruntimeid'];
        $rawExtraData = $fields['rawextradata'];
        $body = <<<'CPP'
#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/@DESCRIPTOR_INCLUDE@>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

struct NetworkItemStack {
CPP;
        $body .= "\n";
        $body .= '    int32_t ' . $stackId . " = 0;\n    NetworkItemDescriptor descriptor;\n\n";
        $body .= "    void write(BinaryStream& stream) const {\n";
        $body .= '        stream.writeShort(static_cast<int16_t>(descriptor.' . $id . "));\n";
        $body .= '        stream.writeUnsignedShort(descriptor.' . $count . ");\n";
        $body .= '        stream.writeUnsignedVarInt(descriptor.' . $meta . ");\n";
        $body .= '        const bool hasId = ' . $stackId . " != 0;\n        stream.writeBool(hasId);\n        if (hasId) {\n";
        $body .= '            stream.writeVarInt(' . $stackId . ");\n        }\n";
        $body .= '        stream.writeUnsignedVarInt(static_cast<uint32_t>(descriptor.' . $blockRuntimeId . "));\n";
        $body .= '        stream.writeString(descriptor.' . $rawExtraData . ");\n    }\n\n";
        $body .= "    void read(ReadOnlyBinaryStream& stream) {\n        descriptor = NetworkItemDescriptor();\n";
        $body .= '        descriptor.' . $id . " = stream.readShort().value_or(0);\n";
        $body .= '        descriptor.' . $count . " = stream.readUnsignedShort().value_or(0);\n";
        $body .= '        descriptor.' . $meta . " = stream.readUnsignedVarInt().value_or(0);\n";
        $body .= '        ' . $stackId . " = stream.readBool().value_or(false) ? stream.readVarInt().value_or(0) : 0;\n";
        $body .= '        descriptor.' . $blockRuntimeId . " = stream.readUnsignedVarInt().value_or(0);\n";
        $body .= '        if (!stream.readString(descriptor.' . $rawExtraData . ")) {\n            descriptor." . $rawExtraData . ".clear();\n        }\n    }\n\n";
        $body .= "};\n";
        return str_replace('@DESCRIPTOR_INCLUDE@', $descriptorInclude, $body);
    }

    private function networkItemPropertyType(array $property): string
    {
        return match (strtolower((string) ($property['name'] ?? ''))) {
            'count' => 'uint16_t',
            'meta' => 'uint32_t',
            default => $this->runtimePropertyType($property),
        };
    }
}
