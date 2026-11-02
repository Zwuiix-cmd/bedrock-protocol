<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\translation;

use PhpParser\Node;
use kodiak\bedrockprotocol\generator\CPPName;

trait CallTranslationMethods
{
private function methodCall(Node\Expr\MethodCall $call, array $declaration, ?array $method): string
    {
        if ($call->var instanceof Node\Expr\Variable && $call->var->name === 'this') {
            $name = $call->name instanceof Node\Identifier ? $call->name->toString() : 'call';
            $inlined = $this->inlineMethodCall($call, $declaration, $method);
            if ($inlined !== null) {
                return $inlined;
            }
            $this->recordUnsupported($declaration, $method ?? ['name' => 'expression', 'line' => $call->getStartLine()], 'call to helper method not emitted: ' . $name);
            return 'std::monostate{}';
        }
        if ($call->var instanceof Node\Expr\New_ && $call->var->class instanceof Node\Name) {
            $class = $call->var->class->getLast();
            $name = $call->name instanceof Node\Identifier ? $call->name->toString() : 'call';
            $alias = $this->catalog->staticCallAlias($class, $name);
            if ($alias !== null) {
                $arguments = array_map(
                    fn (Node $argument): string => $this->argumentExpression($argument, $declaration, $method),
                    $call->args,
                );
                foreach ($arguments as $index => $argument) {
                    $alias = str_replace('%ARG' . $index . '%', $argument, $alias);
                }
                return $alias;
            }
        }
        $object = $this->expression($call->var, $declaration, $method);
        $name = $call->name instanceof Node\Identifier ? $call->name->toString() : 'call';
        $access = $this->accessOperator($call->var, $declaration);
        $variableType = $call->var instanceof Node\Expr\Variable
            ? ($this->expressionTypes[(string) $call->var->name] ?? null)
            : null;
        if ($access === '.' && $variableType !== null) {
            $referenced = $this->findDeclaration(CPPName::type($variableType));
            if ($referenced !== null && ($referenced['kind'] ?? null) === 'class') {
                $access = '->';
            }
        }
        $isThisProperty = $call->var instanceof Node\Expr\PropertyFetch
            && $call->var->var instanceof Node\Expr\Variable
            && $call->var->var->name === 'this';
        if ($access === '.'
            && !$isThisProperty
            && in_array(strtolower($name), ['read', 'write'], true)
            && ($variableType === null || strtolower((string) $variableType) === 'array' || CPPName::type($variableType) === 'Array')) {
            $access = '->';
        }
        if (count($call->args) > 1 && in_array(strtolower($name), ['read', 'write'], true)) {
            return $object . $access . $name . '(stream)';
        }
        $objectType = $this->expressionType($call->var, $declaration);
        $variantType = $objectType === null ? null : $this->templateArguments($this->cppType($objectType), 'std::variant');
        if ($variantType !== null && $variantType !== []) {
            if (in_array($name, ['read', 'decode', 'write', 'encode'], true)) {
                $methodName = in_array($name, ['read', 'decode'], true) ? 'read' : 'write';
                return 'std::visit([&](auto& value) { if (value) value->' . $methodName . '(stream); }, ' . $object . ')';
            }
            if ($name === 'getTypeId') {
                return 'std::visit([&](const auto& value) -> int32_t { return value ? std::remove_cvref_t<decltype(*value)>::ID : 0; }, ' . $object . ')';
            }
            if ($name === 'getActionType') {
                return 'std::visit([&](const auto& value) -> int32_t { return value ? value->getActionType() : 0; }, ' . $object . ')';
            }
        }
        if (in_array(strtolower($name), ['toargb', 'torgba'], true)) {
            return 'static_cast<uint32_t>(' . $object . ')';
        }
        if (strtolower($name) === 'isverified') {
            return $object . $this->accessOperator($call->var, $declaration) . 'isVerified';
        }
        if (strtolower($name) === 'setverified') {
            return $object . $this->accessOperator($call->var, $declaration) . 'isVerified = ' . $this->arguments($call->args, $declaration, $method);
        }
        if ($name === 'toOrdinal' || $name === 'value') {
            if ($this->isStringEnumProperty($call->var, $declaration)) {
                $this->recordUnsupported($declaration, $method ?? ['name' => 'expression', 'line' => $call->getStartLine()], 'string-backed enum ordinal conversion');
                return 'std::monostate{}';
            }
            return 'magic_enum::enum_integer(' . $object . ')';
        }
        if (in_array(strtolower($name), ['getx', 'gety', 'getz'], true)) {
            return $object . $this->accessOperator($call->var, $declaration) . strtolower(substr($name, 3));
        }
        if ($name === 'getTypeId') {
            $objectType = $this->expressionType($call->var, $declaration);
            if ($objectType !== null && CPPName::type($objectType) === 'RecipeWithTypeId') {
                return $object . $this->accessOperator($call->var, $declaration) . 'typeId';
            }
            return $object . $this->accessOperator($call->var, $declaration) . 'getTypeId()';
        }
        $encodedValue = $this->encodedValueWrite($call, $declaration, $method);
        if ($encodedValue !== null) {
            return $encodedValue;
        }
        $getter = $this->getterProperty($call, $declaration);
        if ($getter !== null) {
            if ($call->var instanceof Node\Expr\Variable
                && ($this->expressionAliases[$call->var->name] ?? null) === '(*this)') {
                return $getter;
            }
            return $object . $this->accessOperator($call->var, $declaration) . $getter;
        }
        $binaryBufferVariable = $call->var instanceof Node\Expr\Variable
            && (($this->expressionTypes[(string) $call->var->name] ?? null) !== null)
            && str_ends_with(CPPName::type((string) $this->expressionTypes[(string) $call->var->name]), 'Reader')
                || ($call->var instanceof Node\Expr\Variable
                    && str_ends_with(CPPName::type((string) ($this->expressionTypes[(string) $call->var->name] ?? '')), 'Writer'));
        if ($call->var instanceof Node\Expr\Variable
            && (in_array($call->var->name, ['in', 'out', 'connRequestReader', 'connRequestWriter'], true) || $binaryBufferVariable)) {
            $isWriter = in_array($call->var->name, ['out', 'connRequestWriter'], true)
                || str_ends_with(CPPName::type((string) ($this->expressionTypes[(string) $call->var->name] ?? '')), 'Writer');
            $name = match ($name) {
                'getData' => $isWriter ? 'getBuffer' : 'data',
                'getOffset' => 'position',
                'setOffset' => 'seek',
                default => $name,
            };
            $callbackName = null;
            $callbackEnumType = null;
            $callbackHadType = false;
            $callbackPreviousType = null;
            if ($name === 'writeOptional'
                && isset($call->args[0], $call->args[1])
                && $call->args[0] instanceof Node\Arg
                && $call->args[1] instanceof Node\Arg
                && $call->args[1]->value instanceof Node\Expr\Closure
                && $call->args[1]->value->params !== []) {
                $callbackEnumType = $this->enumPropertyType($call->args[0]->value, $declaration)
                    ?? $this->assignmentCPPType($call->args[0]->value, $declaration);
                if (is_string($callbackEnumType)) {
                    $callbackEnumType = preg_replace('/^std::optional<(.+)>$/', '$1', $callbackEnumType) ?? $callbackEnumType;
                    $callbackName = (string) $call->args[1]->value->params[0]->var->name;
                    $callbackHadType = array_key_exists($callbackName, $this->expressionTypes);
                    $callbackPreviousType = $this->expressionTypes[$callbackName] ?? null;
                    $this->expressionTypes[$callbackName] = $callbackEnumType;
                }
            }
            $arguments = array_map(fn (Node $argument): string => $this->argumentExpression($argument, $declaration, $method), $call->args);
            if ($callbackName !== null && $callbackEnumType !== null && isset($arguments[1])) {
                $arguments[1] = preg_replace(
                    '/(stream\.write(?:Unsigned|Signed)?(?:Byte|Short|Int|Long|VarInt|VarLong)\()'
                        . preg_quote($callbackName, '/') . '(\))/',
                    '$1magic_enum::enum_integer(' . $callbackName . ')$2',
                    $arguments[1],
                ) ?? $arguments[1];
            }
            if ($callbackName !== null) {
                if ($callbackHadType) {
                    $this->expressionTypes[$callbackName] = $callbackPreviousType;
                } else {
                    unset($this->expressionTypes[$callbackName]);
                }
            }
            if (str_starts_with(strtolower($name), 'write')) {
                foreach ($call->args as $index => $argument) {
                    if ($argument instanceof Node\Arg && $this->isStringEnumProperty($argument->value, $declaration)) {
                        $arguments[$index] = $this->stringEnumToString(
                            $argument->value,
                            $arguments[$index],
                            $declaration,
                        );
                    } else {
                        $arguments[$index] = $this->enumStorageCast($argument, $arguments[$index], $declaration);
                    }
                }
            }
            if ($name === 'seek' && count($arguments) === 1) {
                $arguments[] = 'false';
            }
            $result = $object . '.' . CPPName::identifier($name) . '(' . implode(', ', $arguments) . ')';
            if ($name === 'readByteArray' && $arguments !== []) {
                return $result . '.value_or("")';
            }
            return $result;
        }
        if ($name === 'encode') {
            return $object . $this->accessOperator($call->var, $declaration) . 'write(' . $this->arguments($call->args, $declaration, $method) . ')';
        }
        if ($name === 'decode') {
            return $object . $this->accessOperator($call->var, $declaration) . 'read(' . $this->arguments($call->args, $declaration, $method) . ')';
        }
        if ($name !== 'write' && (($call->var instanceof Node\Expr\Variable
                && !isset($this->expressionTypes[(string) $call->var->name])
                && !isset($this->expressionAliases[(string) $call->var->name]))
            || $this->isUnknownProperty($call->var, $declaration))) {
            $this->recordUnsupported($declaration, $method ?? ['name' => 'expression', 'line' => $call->getStartLine()], 'method call on unknown generated value: ' . $name);
            return 'std::monostate{}';
        }
        if ($objectType !== null) {
            if (CPPName::type($objectType) === 'Color' && in_array(strtolower($name), ['toargb', 'torgba'], true)) {
                return 'static_cast<uint32_t>(' . $object . ')';
            }
            if (CPPName::type($objectType) === 'SkinData' && strtolower($name) === 'setverified') {
                return $object . $this->accessOperator($call->var, $declaration) . 'isVerified = ' . $this->arguments($call->args, $declaration, $method);
            }
            $getter = $this->catalog->getterAlias(CPPName::type($objectType), $name);
            if ($getter !== null) {
                return $object . $this->accessOperator($call->var, $declaration) . $getter;
            }
            $methodAlias = $this->catalog->methodAlias(CPPName::type($objectType), $name);
            if ($methodAlias !== null) {
                return $object . $this->accessOperator($call->var, $declaration) . $methodAlias . '(' . $this->arguments($call->args, $declaration, $method) . ')';
            }
        }
        if ($name !== 'write') {
        $this->recordUnsupported($declaration, $method ?? ['name' => 'expression', 'line' => $call->getStartLine()], 'method is not part of the generated type interface: ' . $name);
            return 'std::monostate{}';
        }
        return $object . $access . CPPName::identifier($name) . '(' . $this->arguments($call->args, $declaration, $method) . ')';
    }

private function encodedValueWrite(Node\Expr\MethodCall $call, array $declaration, ?array $method): ?string
    {
        $name = $call->name instanceof Node\Identifier ? strtolower($call->name->toString()) : '';
        $argument = $call->args[0] ?? null;
        if ($name !== 'writebytearray' || !$argument instanceof Node\Arg || !$argument->value instanceof Node\Expr\MethodCall) {
            return null;
        }
        $encodedCall = $argument->value;
        $encodedName = $encodedCall->name instanceof Node\Identifier ? $encodedCall->name->toString() : '';
        $type = $this->expressionType($encodedCall->var, $declaration);
        if ($type === null) {
            return null;
        }
        $writer = $this->catalog->valueWriter(CPPName::type($type), $encodedName);
        if ($writer === null) {
            return null;
        }
        $value = $this->expression($encodedCall->var, $declaration, $method);
        if ($this->isOptionalProperty($encodedCall->var, $declaration) || $this->isOptionalExpression($encodedCall->var)) {
            $value .= '.value()';
        }
        return rtrim(str_replace('%VALUE%', $value, $writer), "; \t");
    }

private function inlineMethodCall(Node\Expr\MethodCall $call, array $declaration, ?array $method): ?string
    {
        $translated = $this->withInlinedMethodContext(
            $call,
            $declaration,
            $method,
            function (array $helper) use ($declaration): ?array {
                $lines = [];
                foreach ($helper['body'] ?? [] as $statement) {
                    $translated = $this->statement($statement, $declaration, $helper, 1);
                    if ($translated !== '') {
                        $lines[] = $translated;
                    }
                }
                return [$helper, $lines];
            },
        );
        if (!is_array($translated) || !isset($translated[0], $translated[1]) || $translated[1] === []) {
            return null;
        }
        /** @var array<string, mixed> $helper */
        $helper = $translated[0];
        /** @var list<string> $lines */
        $lines = $translated[1];
        $returnType = (string) ($helper['returnType'] ?? 'void');
        $cppReturnType = $returnType === '' || $returnType === 'void'
            ? 'void'
            : $this->cppType($returnType, false, $helper['doc'] ?? null);
        $body = implode("\n", $lines);
        return $cppReturnType === 'void'
            ? "([&]() {\n" . $body . "\n}())"
            : '([&]() -> ' . $cppReturnType . " {\n" . $body . "\n}())";
    }

private function withInlinedMethodContext(
        Node\Expr\MethodCall $call,
        array $declaration,
        ?array $method,
        callable $callback,
    ): mixed {
        $name = $call->name instanceof Node\Identifier ? $call->name->toString() : '';
        if ($name === '' || isset($this->inlinedMethods[$name])) {
            return null;
        }

        $helper = null;
        foreach ($declaration['methods'] ?? [] as $candidate) {
            if (($candidate['name'] ?? null) === $name && !($candidate['static'] ?? false)) {
                $helper = $candidate;
                break;
            }
        }
        if (!is_array($helper) || count($helper['parameters'] ?? []) !== count($call->args)) {
            return null;
        }

        $previousTypes = $this->expressionTypes;
        $previousAliases = $this->expressionAliases;
        $previousOptionals = $this->expressionOptionals;
        $this->inlinedMethods[$name] = true;
        try {
            foreach ($helper['parameters'] as $index => $parameter) {
                $parameterName = (string) ($parameter['name'] ?? '');
                $argument = $call->args[$index] ?? null;
                if ($parameterName === '' || !$argument instanceof Node\Arg) {
                    return null;
                }
                $this->expressionAliases[$parameterName] = $this->expression($argument->value, $declaration, $method);
                $type = (string) ($parameter['type'] ?? '');
                if ($type !== '') {
                    $this->expressionTypes[$parameterName] = $type;
                }
                $this->expressionOptionals[$parameterName] = (bool) ($parameter['nullable'] ?? false)
                    || $this->nullableType($type);
            }
            return $callback($helper);
        } finally {
            unset($this->inlinedMethods[$name]);
            $this->expressionTypes = $previousTypes;
            $this->expressionAliases = $previousAliases;
            $this->expressionOptionals = $previousOptionals;
        }
    }

private function nullsafeMethodCall(Node\Expr\NullsafeMethodCall $call, array $declaration, ?array $method): string
    {
        $object = $this->expression($call->var, $declaration, $method);
        $name = $call->name instanceof Node\Identifier ? $call->name->toString() : 'call';
        $objectType = $this->expressionType($call->var, $declaration);
        if ($objectType !== null) {
            $getter = $this->catalog->getterAlias(CPPName::type($objectType), $name);
            if ($getter !== null) {
                return $object . ' ? std::optional{' . $object . '->' . $getter . '} : std::nullopt';
            }
        }
        $this->recordUnsupported($declaration, $method ?? ['name' => 'expression', 'line' => $call->getStartLine()], 'nullsafe method requires an adapter: ' . $name);
        return 'std::monostate{}';
    }

private function accessOperator(Node $node, array $declaration): string
    {
        if ($node instanceof Node\Expr\Variable) {
            if (isset($this->expressionAliases[$node->name])) {
                $type = $this->expressionTypes[(string) $node->name] ?? null;
                return $type === null ? '.' : $this->accessOperatorForType($type);
            }
            $type = $this->expressionTypes[(string) $node->name] ?? null;
            if ($type !== null) {
                return $this->accessOperatorForType($type);
            }
            return '.';
        }
        if ($node instanceof Node\Expr\PropertyFetch && $node->var instanceof Node\Expr\Variable && $node->var->name === 'this') {
            $name = $node->name instanceof Node\Identifier ? $node->name->toString() : '';
            foreach ($declaration['properties'] as $property) {
                if ($property['name'] !== $name) {
                    continue;
                }
                $type = (string) ($property['type'] ?? '');
                $docType = $this->docType($property['doc'] ?? null);
                return $this->accessOperatorForType($type !== '' ? $type : ($docType ?? ''));
            }
        }
        if ($node instanceof Node\Expr\ArrayDimFetch) {
            $elementType = $this->expressionType($node, $declaration);
            if ($elementType !== null) {
                return $this->accessOperatorForType($elementType);
            }
        }
        if ($node instanceof Node\Expr\PropertyFetch) {
            $propertyType = $this->expressionType($node, $declaration);
            if ($propertyType !== null) {
                return $this->accessOperatorForType($propertyType);
            }
            $objectType = $this->expressionType($node->var, $declaration);
            $propertyName = $node->name instanceof Node\Identifier ? $node->name->toString() : '';
            if ($objectType !== null) {
                $referenced = $this->findDeclaration(CPPName::type($objectType));
                foreach ($referenced['properties'] ?? [] as $property) {
                    if (($property['name'] ?? null) !== $propertyName) {
                        continue;
                    }
                    $type = (string) ($property['type'] ?? '');
                    return $this->accessOperatorForType($type);
                }
            }
        }
        return '.';
    }

private function accessOperatorForType(string $type): string
    {
        if ($type === '') {
            return '.';
        }
        if (str_starts_with(ltrim($type, '?'), 'std::optional<std::shared_ptr<')) {
            return '.value()->';
        }
        if (str_starts_with(ltrim($type, '?'), 'std::shared_ptr<')) {
            return '->';
        }
        $cppType = $this->cppType($type);
        if (str_contains($cppType, 'std::optional<std::shared_ptr<')) {
            return '.value()->';
        }
        if (str_starts_with($cppType, 'std::optional<')) {
            return '.value().';
        }
        if (str_starts_with($cppType, 'std::shared_ptr<')) {
            return '->';
        }
        return '.';
    }

private function isPointerLike(string $type): bool
    {
        $cppType = $this->cppType($type);
        return str_starts_with($cppType, 'std::shared_ptr<') || str_contains($cppType, 'std::optional<std::shared_ptr<');
    }

private function getterProperty(Node\Expr\MethodCall $call, array $declaration): ?string
    {
        $name = $call->name instanceof Node\Identifier ? $call->name->toString() : '';
        if (!str_starts_with($name, 'get') || strlen($name) <= 3) {
            return null;
        }

        $type = null;
        if ($call->var instanceof Node\Expr\Variable) {
            $type = $this->expressionTypes[(string) $call->var->name] ?? null;
        } elseif ($call->var instanceof Node\Expr\PropertyFetch
            && $call->var->var instanceof Node\Expr\Variable
            && $call->var->var->name === 'this') {
            $propertyName = $call->var->name instanceof Node\Identifier ? $call->var->name->toString() : '';
            foreach ($declaration['properties'] as $property) {
                if ($property['name'] === $propertyName) {
                    $type = $property['type'] ?? null;
                    break;
                }
            }
        }
        if ($type === null) {
            return null;
        }

        $referenced = $this->findDeclaration(CPPName::type((string) $type));
        if ($referenced === null) {
            return null;
        }
        $suffix = strtolower(substr($name, 3));
        foreach ($referenced['properties'] as $property) {
            if (!$property['static'] && strtolower((string) $property['name']) === $suffix) {
                return CPPName::identifier((string) $property['name']);
            }
        }
        return null;
    }

private function expressionType(Node\Expr $expression, array $declaration): ?string
    {
        if ($expression instanceof Node\Expr\Variable) {
            return $this->expressionTypes[(string) $expression->name] ?? null;
        }
        if ($expression instanceof Node\Expr\PropertyFetch
            && $expression->var instanceof Node\Expr\Variable
            && $expression->var->name === 'this') {
            $name = $expression->name instanceof Node\Identifier ? $expression->name->toString() : '';
            foreach ($declaration['properties'] as $property) {
                if (($property['name'] ?? null) === $name) {
                    $type = (string) ($property['type'] ?? '');
                    return ($type === '' || ltrim($type, '?') === 'array' || $type === 'mixed')
                        ? ($this->docType($property['doc'] ?? null) ?? ($type !== '' ? $type : null))
                        : $type;
                }
            }
        }
        if ($expression instanceof Node\Expr\ArrayDimFetch) {
            $arrayType = $this->expressionType($expression->var, $declaration);
            if ($arrayType !== null) {
                return $this->arrayElementType($arrayType, null);
            }
        }
        if ($expression instanceof Node\Expr\PropertyFetch) {
            $objectType = $this->expressionType($expression->var, $declaration);
            $propertyName = $expression->name instanceof Node\Identifier ? $expression->name->toString() : '';
            if ($objectType !== null) {
                $referenced = $this->findDeclaration(CPPName::type($objectType));
                foreach ($referenced['properties'] ?? [] as $property) {
                    if (($property['name'] ?? null) !== $propertyName) {
                        continue;
                    }
                    $type = (string) ($property['type'] ?? '');
                    return ($type === '' || ltrim($type, '?') === 'array' || $type === 'mixed')
                        ? ($this->docType($property['doc'] ?? null) ?? ($type !== '' ? $type : null))
                        : $type;
                }
            }
        }
        if ($expression instanceof Node\Expr\MethodCall) {
            $objectType = $this->expressionType($expression->var, $declaration);
            if ($objectType !== null) {
                $referenced = $this->findDeclaration(CPPName::type($objectType));
                $methodName = $expression->name instanceof Node\Identifier ? strtolower($expression->name->toString()) : '';
                if ($referenced !== null && str_starts_with($methodName, 'get')) {
                    $suffix = substr($methodName, 3);
                    foreach ($referenced['properties'] ?? [] as $property) {
                        if (!$property['static'] && strtolower((string) $property['name']) === $suffix) {
                            return $property['type'] ?? $this->docType($property['doc'] ?? null);
                        }
                    }
                }
            }
        }
        return null;
    }

private function assignmentCPPType(Node $target, array $declaration): ?string
    {
        if (!$target instanceof Node\Expr) {
            return null;
        }
        $enumType = $this->enumPropertyType($target, $declaration);
        if ($enumType !== null) {
            return $enumType;
        }
        $emittedType = $this->emittedExpressionType($target, $declaration);
        if ($emittedType !== null) {
            return $emittedType;
        }
        $type = $this->expressionType($target, $declaration);
        return $type === null ? null : $this->cppType($type);
    }
}
