<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\translation;

use PhpParser\Node;
use kodiak\bedrockprotocol\generator\CPPName;

trait PropertyCodecInferenceMethods
{
private function inferPropertyCodecs(): void
    {
        foreach ($this->declarations as $declaration) {
            foreach ($declaration['methods'] ?? [] as $method) {
                $statements = $method['body'] ?? [];
                if (!is_array($statements)) {
                    continue;
                }
                $variableTypes = [];
                $variableCodecs = [];
                foreach ($statements as $statement) {
                    if ($statement instanceof Node) {
                        $this->collectConstructedVariables($statement, $variableTypes);
                        $this->collectVariableCodecs($statement, $variableCodecs, $declaration);
                    }
                }
                foreach ($statements as $statement) {
                    if ($statement instanceof Node) {
                        $this->inferPropertyCodecsFromNode($statement, $declaration, $variableTypes, $variableCodecs);
                    }
                }
            }
        }
    }

private function collectConstructedVariables(Node $node, array &$variableTypes): void
    {
        if ($node instanceof Node\Expr\Assign
            && $node->var instanceof Node\Expr\Variable
            && $node->expr instanceof Node\Expr\New_
            && $node->expr->class instanceof Node\Name) {
            $variableTypes[(string) $node->var->name] = CPPName::type($node->expr->class->getLast());
        }
        foreach ($node->getSubNodeNames() as $name) {
            $child = $node->$name;
            if ($child instanceof Node) {
                $this->collectConstructedVariables($child, $variableTypes);
            } elseif (is_array($child)) {
                foreach ($child as $item) {
                    if ($item instanceof Node) {
                        $this->collectConstructedVariables($item, $variableTypes);
                    }
                }
            }
        }
    }

private function collectVariableCodecs(Node $node, array &$variableCodecs, array $declaration): void
    {
        if ($node instanceof Node\Expr\Assign
            && $node->var instanceof Node\Expr\Variable) {
            $codec = $this->codecExpression($node->expr, $variableCodecs, $declaration);
            if ($codec !== null) {
                $variableCodecs[(string) $node->var->name] = $codec;
            }
        }
        if ($node instanceof Node\Expr\Assign
            && $node->var instanceof Node\Expr\ArrayDimFetch
            && $node->var->dim === null
            && $node->var->var instanceof Node\Expr\Variable) {
            $codec = $this->codecExpression($node->expr, $variableCodecs, $declaration);
            if ($codec !== null) {
                $codec['list'] = true;
                $variableCodecs[(string) $node->var->var->name] = $codec;
            }
        }
        foreach ($node->getSubNodeNames() as $name) {
            $child = $node->$name;
            if ($child instanceof Node) {
                $this->collectVariableCodecs($child, $variableCodecs, $declaration);
            } elseif (is_array($child)) {
                foreach ($child as $item) {
                    if ($item instanceof Node) {
                        $this->collectVariableCodecs($item, $variableCodecs, $declaration);
                    }
                }
            }
        }
    }

private function inferPropertyCodecsFromNode(Node $node, array $declaration, array $variableTypes, array $variableCodecs): void
    {
        if ($node instanceof Node\Expr\Assign) {
            $target = $node->var;
            $isElementAssignment = $target instanceof Node\Expr\ArrayDimFetch;
            if ($target instanceof Node\Expr\ArrayDimFetch) {
                $target = $target->var;
            }
            if ($target instanceof Node\Expr\PropertyFetch && $target->name instanceof Node\Identifier) {
                $owner = $this->propertyOwnerName($target->var, $declaration, $variableTypes);
                $codec = $this->codecExpression($node->expr, $variableCodecs, $declaration);
                if ($owner !== null && $codec !== null) {
                    if ($isElementAssignment) {
                        $owner = CPPName::type($owner);
                        $this->inferredPropertyTypes[$owner][$target->name->toString()] ??= $codec['type'];
                    } else {
                        $this->registerPropertyCodec($owner, $target->name->toString(), $codec);
                    }
                }
            }
        }
        if ($node instanceof Node\Expr\New_) {
            $this->inferConstructorPropertyCodecs($node, $variableCodecs, $declaration);
        }
        foreach ($node->getSubNodeNames() as $name) {
            $child = $node->$name;
            if ($child instanceof Node) {
                $this->inferPropertyCodecsFromNode($child, $declaration, $variableTypes, $variableCodecs);
            } elseif (is_array($child)) {
                foreach ($child as $item) {
                    if ($item instanceof Node) {
                        $this->inferPropertyCodecsFromNode($item, $declaration, $variableTypes, $variableCodecs);
                    }
                }
            }
        }
    }

private function inferConstructorPropertyCodecs(Node\Expr\New_ $node, array $variableCodecs, array $declaration): void
    {
        if (!$node->class instanceof Node\Name) {
            return;
        }
        $ownerName = $node->class->getLast();
        $owner = in_array($ownerName, ['self', 'static'], true)
            ? CPPName::type((string) $declaration['name'])
            : CPPName::type($ownerName);
        $referenced = $this->findDeclaration($owner);
        if ($referenced === null) {
            return;
        }
        $properties = array_values(array_filter(
            $referenced['properties'] ?? [],
            static fn (array $property): bool => !($property['static'] ?? false),
        ));
        $constructorParameters = [];
        foreach ($referenced['methods'] ?? [] as $method) {
            if (($method['name'] ?? null) === '__construct') {
                foreach ($method['parameters'] ?? [] as $parameter) {
                    $constructorParameters[] = (string) ($parameter['name'] ?? '');
                }
                break;
            }
        }
        $propertiesByName = [];
        foreach ($properties as $property) {
            $propertiesByName[(string) ($property['name'] ?? '')] = $property;
        }
        foreach ($node->args as $index => $argument) {
            if (!$argument instanceof Node\Arg) {
                continue;
            }
            $property = isset($constructorParameters[$index], $propertiesByName[$constructorParameters[$index]])
                ? $propertiesByName[$constructorParameters[$index]]
                : ($properties[$index] ?? null);
            if (!is_array($property)) {
                continue;
            }
            $this->registerPropertyOrder($owner, (string) $property['name']);
            $codec = $this->codecExpression($argument->value, $variableCodecs, $declaration);
            if ($codec !== null) {
                if (($codec['list'] ?? false) === true) {
                    $this->registerPropertyElementCodec($owner, (string) $property['name'], $codec);
                } else {
                    $this->registerPropertyCodec($owner, (string) $property['name'], $codec);
                }
            }
        }
    }

private function propertyOwnerName(Node\Expr $node, array $declaration, array $variableTypes): ?string
    {
        if ($node instanceof Node\Expr\Variable) {
            if ($node->name === 'this') {
                return CPPName::type((string) $declaration['name']);
            }
            return $variableTypes[(string) $node->name] ?? null;
        }
        if ($node instanceof Node\Expr\ArrayDimFetch) {
            return $this->propertyOwnerName($node->var, $declaration, $variableTypes);
        }
        if ($node instanceof Node\Expr\PropertyFetch && $node->name instanceof Node\Identifier) {
            $owner = $this->propertyOwnerName($node->var, $declaration, $variableTypes);
            if ($owner === null) {
                return null;
            }
            $ownerDeclaration = $this->findDeclaration($owner);
            foreach ($ownerDeclaration['properties'] ?? [] as $property) {
                if (($property['name'] ?? null) !== $node->name->toString()) {
                    continue;
                }
                $type = $this->docType($property['doc'] ?? null) ?? (string) ($property['type'] ?? '');
                if (preg_match('/^(?:array|list)\s*<\s*(?:[^,>]+\s*,\s*)?(.+)>$/i', $type, $matches) === 1) {
                    $type = trim($matches[1]);
                } elseif (str_ends_with($type, '[]')) {
                    $type = substr($type, 0, -2);
                }
                return CPPName::type($type);
            }
        }
        return null;
    }

private function codecExpression(Node $expression, array $variableCodecs, array $declaration): ?array
    {
        if ($expression instanceof Node\Expr\Variable) {
            return $variableCodecs[(string) $expression->name] ?? null;
        }
        if ($expression instanceof Node\Expr\StaticCall) {
            $class = $expression->class instanceof Node\Name ? $expression->class->getLast() : '';
            if ($class === 'self' || $class === 'static') {
                $class = (string) $declaration['name'];
            }
            $name = $expression->name instanceof Node\Identifier ? $expression->name->toString() : '';
            if (strtolower($name) === 'readoptional' && isset($expression->args[1])) {
                return $this->codecExpression($expression->args[1]->value, $variableCodecs, $declaration);
            }
            if (in_array(strtolower($name), ['frompacket', 'fromordinal', 'fromargb', 'fromrgba'], true)
                && isset($expression->args[0])) {
                return $this->codecExpression($expression->args[0]->value, $variableCodecs, $declaration);
            }
            return $this->codecForCall($class, $name);
        }
        if ($expression instanceof Node\Expr\MethodCall) {
            $name = $expression->name instanceof Node\Identifier ? $expression->name->toString() : '';
            if (strtolower($name) === 'readoptional' && isset($expression->args[0])) {
                return $this->codecExpression($expression->args[0]->value, $variableCodecs, $declaration);
            }
            return $this->codecForCall('stream', $name);
        }
        if ($expression instanceof Node\Expr\Cast) {
            return $this->codecExpression($expression->expr, $variableCodecs, $declaration);
        }
        if ($expression instanceof Node\Expr\Assign) {
            return $this->codecExpression($expression->expr, $variableCodecs, $declaration);
        }
        if ($expression instanceof Node\Expr\Closure) {
            foreach ($expression->stmts as $statement) {
                if ($statement instanceof Node\Stmt\Return_) {
                    $codec = $this->codecExpression($statement->expr, $variableCodecs, $declaration);
                    if ($codec !== null) {
                        return $codec;
                    }
                }
            }
            return null;
        }
        if ($expression instanceof Node\Expr\ArrowFunction) {
            return $this->codecExpression($expression->expr, $variableCodecs, $declaration);
        }
        if ($expression instanceof Node\Expr\Ternary) {
            return $this->codecExpression($expression->if ?? $expression->cond, $variableCodecs, $declaration)
                ?? $this->codecExpression($expression->else, $variableCodecs, $declaration);
        }
        return null;
    }

private function codecForCall(string $class, string $name): ?array
    {
        $class = strtolower($class);
        $name = strtolower($name);
        $read = null;
        $write = null;
        $type = null;
        if (str_contains($name, 'actoruniqueid')) {
            return ['type' => 'int64_t', 'read' => 'stream.readActorUniqueID().value_or(0)', 'write' => 'stream.writeActorUniqueID(%VALUE%);'];
        }
        if (str_contains($name, 'actorruntimeid')) {
            return ['type' => 'uint64_t', 'read' => 'stream.readActorRuntimeID().value_or(0)', 'write' => 'stream.writeActorRuntimeID(%VALUE%);'];
        }
        if ($class === 'varint') {
            $read = match (true) {
                str_contains($name, 'unsignedlong') => 'stream.readUnsignedVarLong().value_or(0)',
                str_contains($name, 'signedlong') => 'stream.readVarLong().value_or(0)',
                str_contains($name, 'unsignedint') => 'stream.readUnsignedVarInt().value_or(0)',
                str_contains($name, 'signedint') => 'stream.readVarInt().value_or(0)',
                default => null,
            };
            $type = match (true) {
                str_contains($name, 'unsignedlong') => 'uint64_t',
                str_contains($name, 'signedlong') => 'int64_t',
                str_contains($name, 'unsignedint') => 'uint32_t',
                str_contains($name, 'signedint') => 'int32_t',
                default => null,
            };
            $write = match (true) {
                str_contains($name, 'unsignedlong') => 'stream.writeUnsignedVarLong(%VALUE%);',
                str_contains($name, 'signedlong') => 'stream.writeVarLong(%VALUE%);',
                str_contains($name, 'unsignedint') => 'stream.writeUnsignedVarInt(%VALUE%);',
                str_contains($name, 'signedint') => 'stream.writeVarInt(%VALUE%);',
                default => null,
            };
        } elseif ($class === 'le' || $class === 'be' || $class === 'stream') {
            if (!str_starts_with($name, 'read') && !str_starts_with($name, 'write')) {
                return null;
            }
            if ($name === 'readbytearray') {
                return ['type' => 'std::string', 'read' => 'stream.readByteArray(0).value_or("")', 'write' => 'stream.writeByteArray(%VALUE%);'];
            }
            $unsigned = str_contains($name, 'unsigned');
            $long = str_contains($name, 'long');
            $short = str_contains($name, 'short');
            $byte = str_contains($name, 'byte');
            $suffix = $class === 'be' ? 'BE' : '';
            if (str_contains($name, 'float')) {
                $type = 'float';
                $read = 'stream.readFloat' . $suffix . '().value_or(0.0f)';
                $write = 'stream.writeFloat' . $suffix . '(%VALUE%);';
            } elseif ($long) {
                $type = $unsigned ? 'uint64_t' : 'int64_t';
                $read = $unsigned ? 'stream.readUnsignedLong' . $suffix . '().value_or(0)' : 'stream.readLong' . $suffix . '().value_or(0)';
                $write = $unsigned ? 'stream.writeUnsignedLong' . $suffix . '(%VALUE%);' : 'stream.writeLong' . $suffix . '(%VALUE%);';
            } elseif ($short) {
                $type = $unsigned ? 'uint16_t' : 'int16_t';
                $read = $unsigned ? 'stream.readUnsignedShort' . $suffix . '().value_or(0)' : 'stream.readShort' . $suffix . '().value_or(0)';
                $write = $unsigned ? 'stream.writeUnsignedShort' . $suffix . '(%VALUE%);' : 'stream.writeShort' . $suffix . '(%VALUE%);';
            } elseif ($byte) {
                $type = $unsigned ? 'uint8_t' : 'int8_t';
                $read = $unsigned ? 'stream.readUnsignedByte().value_or(0)' : 'stream.readByte().value_or(0)';
                $write = $unsigned ? 'stream.writeUnsignedByte(%VALUE%);' : 'stream.writeByte(%VALUE%);';
            } elseif (str_contains($name, 'int')) {
                $type = $unsigned ? 'uint32_t' : 'int32_t';
                $read = $unsigned ? 'stream.readUnsignedInt' . $suffix . '().value_or(0)' : 'stream.readInt' . $suffix . '().value_or(0)';
                $write = $unsigned ? 'stream.writeUnsignedInt' . $suffix . '(%VALUE%);' : 'stream.writeInt' . $suffix . '(%VALUE%);';
            }
        } elseif ($class === 'byte') {
            $type = str_contains($name, 'unsigned') ? 'uint8_t' : 'int8_t';
            $read = str_contains($name, 'unsigned') ? 'stream.readUnsignedByte().value_or(0)' : 'stream.readByte().value_or(0)';
            $write = str_contains($name, 'unsigned') ? 'stream.writeUnsignedByte(%VALUE%);' : 'stream.writeByte(%VALUE%);';
        } elseif ($class === 'commontypes') {
            $mapped = $this->catalog->streamMethod('CommonTypes', $name);
            if ($mapped !== null) {
                $mappedCodec = $this->codecForMappedStreamMethod($mapped);
                if ($mappedCodec !== null) {
                    return $mappedCodec;
                }
            }
            $simple = str_replace(['get', 'put'], '', $name);
            if ($simple === 'vector2') {
                return ['type' => 'glm::vec2', 'read' => 'stream.readVec2().value_or(glm::vec2(0.0f))', 'write' => 'stream.writeVec2(%VALUE%);'];
            }
            if ($simple === 'vector3') {
                return ['type' => 'glm::vec3', 'read' => 'stream.readVec3().value_or(glm::vec3(0.0f))', 'write' => 'stream.writeVec3(%VALUE%);'];
            }
            if ($simple === 'blockposition') {
                return ['type' => 'glm::ivec3', 'read' => 'stream.readSignedBlockPos().value_or(glm::ivec3(0))', 'write' => 'stream.writeSignedBlockPos(%VALUE%);'];
            }
            $mapped = $this->codecForCall('stream', $simple);
            if ($mapped !== null) {
                return $mapped;
            }
            if ($name === 'getbool' || $name === 'putbool') {
                return ['type' => 'bool', 'read' => 'stream.readBool().value_or(false)', 'write' => 'stream.writeBool(%VALUE%);'];
            }
            if ($name === 'getstring' || $name === 'putstring') {
                return ['type' => 'std::string', 'read' => 'stream.readString().value_or("")', 'write' => 'stream.writeString(%VALUE%);'];
            }
        }
        return $type !== null && $read !== null && $write !== null ? ['type' => $type, 'read' => $read, 'write' => $write] : null;
    }

    private function codecForMappedStreamMethod(string $method): ?array
    {
        return match ($method) {
            'readBool' => [
                'type' => 'bool',
                'read' => 'stream.readBool().value_or(false)',
                'write' => 'stream.writeBool(%VALUE%);',
            ],
            'writeBool' => [
                'type' => 'bool',
                'read' => 'stream.readBool().value_or(false)',
                'write' => 'stream.writeBool(%VALUE%);',
            ],
            'readString' => [
                'type' => 'std::string',
                'read' => 'stream.readString().value_or("")',
                'write' => 'stream.writeString(%VALUE%);',
            ],
            'writeString' => [
                'type' => 'std::string',
                'read' => 'stream.readString().value_or("")',
                'write' => 'stream.writeString(%VALUE%);',
            ],
            'readVarInt' => [
                'type' => 'int32_t',
                'read' => 'stream.readVarInt().value_or(0)',
                'write' => 'stream.writeVarInt(%VALUE%);',
            ],
            'writeVarInt' => [
                'type' => 'int32_t',
                'read' => 'stream.readVarInt().value_or(0)',
                'write' => 'stream.writeVarInt(%VALUE%);',
            ],
            'readUnsignedVarInt' => [
                'type' => 'uint32_t',
                'read' => 'stream.readUnsignedVarInt().value_or(0)',
                'write' => 'stream.writeUnsignedVarInt(%VALUE%);',
            ],
            'writeUnsignedVarInt' => [
                'type' => 'uint32_t',
                'read' => 'stream.readUnsignedVarInt().value_or(0)',
                'write' => 'stream.writeUnsignedVarInt(%VALUE%);',
            ],
            'readVarLong' => [
                'type' => 'int64_t',
                'read' => 'stream.readVarLong().value_or(0)',
                'write' => 'stream.writeVarLong(%VALUE%);',
            ],
            'writeVarLong' => [
                'type' => 'int64_t',
                'read' => 'stream.readVarLong().value_or(0)',
                'write' => 'stream.writeVarLong(%VALUE%);',
            ],
            'readUnsignedVarLong' => [
                'type' => 'uint64_t',
                'read' => 'stream.readUnsignedVarLong().value_or(0)',
                'write' => 'stream.writeUnsignedVarLong(%VALUE%);',
            ],
            'writeUnsignedVarLong' => [
                'type' => 'uint64_t',
                'read' => 'stream.readUnsignedVarLong().value_or(0)',
                'write' => 'stream.writeUnsignedVarLong(%VALUE%);',
            ],
            default => null,
        };
    }

private function registerPropertyCodec(string $owner, string $property, array $codec): void
    {
        $owner = CPPName::type($owner);
        $this->registerPropertyOrder($owner, $property);
        $this->inferredPropertyTypes[$owner][$property] = $codec['type'];
        $this->inferredPropertyCodecs[$owner][$property] = $codec;
    }

    private function registerPropertyElementCodec(string $owner, string $property, array $codec): void
    {
        $owner = CPPName::type($owner);
        $this->registerPropertyOrder($owner, $property);
        $this->inferredPropertyTypes[$owner][$property] = $codec['type'];
        $this->inferredPropertyElementCodecs[$owner][$property] = [
            'type' => $codec['type'],
            'read' => $codec['read'],
            'write' => $codec['write'],
        ];
    }

    private function registerPropertyOrder(string $owner, string $property): void
    {
        $owner = CPPName::type($owner);
        if (!isset($this->inferredPropertyOrder[$owner])) {
            $this->inferredPropertyOrder[$owner] = [];
        }
        if (!in_array($property, $this->inferredPropertyOrder[$owner], true)) {
            $this->inferredPropertyOrder[$owner][] = $property;
        }
    }

private function orderedProperties(array $declaration): array
    {
        $properties = array_values(array_filter(
            $declaration['properties'] ?? [],
            static fn (array $property): bool => !($property['static'] ?? false),
        ));
        $byName = [];
        foreach ($properties as $property) {
            $byName[(string) ($property['name'] ?? '')] = $property;
        }
        $ordered = [];
        foreach ($this->inferredPropertyOrder[CPPName::type((string) ($declaration['name'] ?? ''))] ?? [] as $name) {
            if (isset($byName[$name])) {
                $ordered[] = $byName[$name];
                unset($byName[$name]);
            }
        }
        foreach ($properties as $property) {
            $name = (string) ($property['name'] ?? '');
            if (isset($byName[$name])) {
                $ordered[] = $property;
                unset($byName[$name]);
            }
        }
        return $ordered;
    }

private function inferredPropertyType(array $declaration, array $property): ?string
    {
        return $this->inferredPropertyTypes[CPPName::type((string) ($declaration['name'] ?? ''))][(string) ($property['name'] ?? '')] ?? null;
    }

    private function inferredPropertyElementCodec(array $declaration, array $property, string $operation): ?string
    {
        return $this->inferredPropertyElementCodecs[
            CPPName::type((string) ($declaration['name'] ?? ''))
        ][(string) ($property['name'] ?? '')][$operation] ?? null;
    }

private function inferredPropertyReadValue(array $declaration, array $property): ?string
    {
        $elementCodec = $this->inferredPropertyElementCodec($declaration, $property, 'read');
        if ($elementCodec !== null) {
            return 'stream.readList(' . $this->callback('', 'return ' . $elementCodec . ';') . ')';
        }
        $codec = $this->inferredPropertyCodec($declaration, $property, 'read');
        if ($codec === null) {
            return null;
        }
        $sourceType = (string) ($property['type'] ?? '');
        if ($this->nullableType($sourceType)) {
            $reader = $this->optionalReadMethod(
                $declaration,
                (string) ($property['name'] ?? ''),
            );
            return 'stream.' . $reader . '(' . $this->callback('', 'return ' . $codec . ';') . ')';
        }
        return $codec;
    }

private function inferredPropertyWriteValue(array $declaration, array $property, string $value): ?string
    {
        $elementCodec = $this->inferredPropertyElementCodec($declaration, $property, 'write');
        if ($elementCodec !== null) {
            return 'stream.writeList(' . $value . ', ' . $this->callback(
                'const auto& item',
                str_replace('%VALUE%', 'item', $elementCodec),
            ) . ');';
        }
        $codec = $this->inferredPropertyCodec($declaration, $property, 'write');
        if ($codec === null) {
            return null;
        }
        $sourceType = (string) ($property['type'] ?? '');
        if ($this->nullableType($sourceType)) {
            $write = str_replace('%VALUE%', 'optionalValue', $codec);
            $writer = $this->optionalWriteMethod(
                $declaration,
                (string) ($property['name'] ?? ''),
            );
            return 'stream.' . $writer . '(' . $value . ', ' . $this->callback('const auto& optionalValue', $write) . ');';
        }
        return str_replace('%VALUE%', $value, $codec);
    }

private function inferredPropertyCodec(array $declaration, array $property, string $operation): ?string
    {
        return $this->inferredPropertyCodecs[CPPName::type((string) ($declaration['name'] ?? ''))][(string) ($property['name'] ?? '')][$operation] ?? null;
    }

private function collectNestedAssignments(Node $node, array &$variables): void
    {
        if ($node instanceof Node\Expr\Assign
            && $node->var instanceof Node\Expr\Variable
            && $node->var->name !== 'this') {
            $variables[] = (string) $node->var->name;
        }
        foreach ($node->getSubNodeNames() as $name) {
            $child = $node->$name;
            if ($child instanceof Node) {
                $this->collectNestedAssignments($child, $variables);
            } elseif (is_array($child)) {
                foreach ($child as $item) {
                    if ($item instanceof Node) {
                        $this->collectNestedAssignments($item, $variables);
                    }
                }
            }
        }
    }
}
