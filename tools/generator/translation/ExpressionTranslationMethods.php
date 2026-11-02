<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\translation;

use PhpParser\Node;
use kodiak\bedrockprotocol\generator\CPPName;

trait ExpressionTranslationMethods
{
public function expression(?Node $expression, array $declaration, ?array $method = null, ?string $expectedType = null): string
    {
        if ($expression === null) {
            return 'std::monostate{}';
        }
        if ($expression instanceof Node\Expr\Throw_) {
            return $this->throwExpression($expression, $declaration, $method);
        }
        if ($expression instanceof Node\Expr\Variable) {
            if ($expression->name === 'this') {
                return '(*this)';
            }
            if (isset($this->expressionAliases[$expression->name])) {
                return $this->expressionAliases[$expression->name];
            }
            if (in_array($expression->name, ['in', 'out'], true)) {
                return 'stream';
            }
            $value = $this->variableName((string) $expression->name, $declaration);
            if ($expectedType !== null && ($this->expressionOptionals[(string) $expression->name] ?? false)) {
                return $value . '.value()';
            }
            return $value;
        }
        if ($expression instanceof Node\Scalar\Int_ || $expression instanceof Node\Scalar\Float_) {
            return (string) $expression->value;
        }
        if ($expression instanceof Node\Scalar\String_) {
            return '"' . $this->escape($expression->value) . '"';
        }
        if ($expression instanceof Node\Expr\ConstFetch) {
            return match (strtolower($expression->name->toString())) {
                'true' => 'true',
                'false' => 'false',
                'null' => 'nullptr',
                'php_int_max' => $expectedType !== null
                    && in_array($expectedType, ['int8_t', 'uint8_t', 'int16_t', 'uint16_t', 'int32_t', 'uint32_t', 'int64_t', 'uint64_t'], true)
                    ? 'std::numeric_limits<' . $expectedType . '>::max()'
                    : 'std::numeric_limits<int64_t>::max()',
                default => CPPName::identifier($expression->name->getLast()),
            };
        }
        if ($expression instanceof Node\Expr\PropertyFetch) {
            if ($expression->var instanceof Node\Expr\Variable
                && ($this->expressionAliases[$expression->var->name] ?? null) === '(*this)') {
                $property = $expression->name instanceof Node\Identifier ? $expression->name->toString() : 'value';
                return $this->coerceVariantAlternative($expression, CPPName::identifier($property), $expectedType, $declaration);
            }
            $object = $this->expression($expression->var, $declaration, $method);
            $property = $expression->name instanceof Node\Identifier ? $expression->name->toString() : 'value';
            if ($property === 'value' && $this->isStringEnumProperty($expression->var, $declaration)) {
                return 'magic_enum::enum_name(' . $object . ')';
            }
            if ($expression->var instanceof Node\Expr\Variable && $expression->var->name === 'this') {
                if (!in_array($property, array_column($declaration['properties'], 'name'), true)) {
                    $this->recordUnsupported($declaration, $method ?? ['name' => 'expression', 'line' => $expression->getStartLine()], 'property belongs to an omitted parent declaration: ' . $property);
                }
                return $this->coerceVariantAlternative($expression, CPPName::identifier($property), $expectedType, $declaration);
            }
            if ($property === 'value') {
                return 'static_cast<std::underlying_type_t<std::remove_cvref_t<decltype(' . $object . ')>>>(' . $object . ')';
            }
            return $this->coerceVariantAlternative(
                $expression,
                $object . $this->accessOperator($expression->var, $declaration) . CPPName::identifier($property),
                $expectedType,
                $declaration,
            );
        }
        if ($expression instanceof Node\Expr\StaticPropertyFetch) {
            $class = $expression->class instanceof Node\Name ? $expression->class->getLast() : '';
            $property = $expression->name instanceof Node\VarLikeIdentifier ? $expression->name->toString() : 'value';
            return CPPName::type($class) . '::' . CPPName::identifier($property);
        }
        if ($expression instanceof Node\Expr\ClassConstFetch) {
            return $this->classConstant($expression, $declaration);
        }
        if ($expression instanceof Node\Expr\MethodCall) {
            return $this->methodCall($expression, $declaration, $method);
        }
        if ($expression instanceof Node\Expr\NullsafeMethodCall) {
            return $this->nullsafeMethodCall($expression, $declaration, $method);
        }
        if ($expression instanceof Node\Expr\StaticCall) {
            return $this->staticCall($expression, $declaration, $method);
        }
        if ($expression instanceof Node\Expr\New_) {
            $className = $expression->class instanceof Node\Name ? $expression->class->getLast() : '';
            if ($className === 'self' || $className === 'static') {
                return CPPName::type($declaration['name']) . '()';
            }
            if ($className === 'Color') {
                $colorArguments = array_map(
                    fn (Node\Arg $argument): string => $this->argumentExpression($argument, $declaration, $method),
                    $expression->args,
                );
                $red = $colorArguments[0] ?? '0';
                $green = $colorArguments[1] ?? '0';
                $blue = $colorArguments[2] ?? '0';
                $alpha = $colorArguments[3] ?? '255';
                return '((static_cast<uint32_t>(' . $alpha . ') << 24) | (static_cast<uint32_t>(' . $red . ') << 16) | (static_cast<uint32_t>(' . $green . ') << 8) | static_cast<uint32_t>(' . $blue . '))';
            }
            if (str_ends_with($className, 'Reader') || str_ends_with($className, 'Writer')) {
                $argument = $expression->args[0] ?? null;
                $value = $argument instanceof Node\Arg
                    ? $this->argumentExpression($argument, $declaration, $method)
                    : null;
                if (str_ends_with($className, 'Reader')) {
                    return $value === null ? 'ReadOnlyBinaryStream()' : 'ReadOnlyBinaryStream(' . $value . ')';
                }
                return 'BinaryStream()';
            }
            $constructedType = $this->catalog->constructor($className);
            if ($constructedType !== null) {
                $codec = $this->catalog->typeCodec($className);
                $arguments = $this->arguments($expression->args, $declaration, $method);
                return (is_array($codec) && ($codec['value'] ?? false) === true)
                    ? $constructedType . '{' . $arguments . '}'
                    : 'std::make_shared<' . $constructedType . '>(' . $arguments . ')';
            }
            $aliasedType = $this->catalog->typeAlias($className);
            $codec = $this->catalog->typeCodec($className);
            if ($aliasedType !== null && is_array($codec) && ($codec['value'] ?? false) === true) {
                $arguments = array_map(
                    fn (Node $argument): string => $this->argumentExpression($argument, $declaration, $method),
                    $expression->args,
                );
                if (count($arguments) === 1) {
                    return $arguments[0];
                }
                return $aliasedType . '{' . implode(', ', $arguments) . '}';
            }
            if ($this->findDeclaration($className) === null) {
                $this->recordUnsupported($declaration, $method ?? ['name' => 'expression', 'line' => $expression->getStartLine()], 'construction of external PHP type');
                return 'std::monostate{}';
            }
            $class = CPPName::type($className);
            return 'std::make_shared<' . $class . '>(' . $this->constructorArguments($expression, $className, $declaration, $method) . ')';
        }
        if ($expression instanceof Node\Expr\Array_) {
            $items = [];
            $expectedItems = $this->templateArguments($expectedType, 'std::tuple');
            foreach ($expression->items as $index => $item) {
                if ($item !== null) {
                    $items[] = $this->expression($item->value, $declaration, $method, $expectedItems[$index] ?? null);
                }
            }
            if ($expectedType !== null && str_starts_with($expectedType, 'std::vector<')) {
                return $expectedType . '{' . implode(', ', $items) . '}';
            }
            return $expectedItems !== []
                ? $expectedType . '{' . implode(', ', $items) . '}'
                : '{' . implode(', ', $items) . '}';
        }
        if ($expression instanceof Node\Expr\FuncCall) {
            $name = $expression->name instanceof Node\Name ? strtolower($expression->name->toString()) : '';
            if ($name === 'count' || $name === 'sizeof') {
                $targetNode = $expression->args[0]->value ?? null;
                $target = $this->expression($targetNode, $declaration, $method);
                if ($targetNode instanceof Node\Expr && $this->isOptionalContainer($targetNode, $declaration)) {
                    $target .= '.value()';
                }
                return $target . '.size()';
            }
            if ($name === 'strlen') {
                $targetNode = $expression->args[0]->value ?? null;
                $target = $this->expression($targetNode, $declaration, $method);
                if ($targetNode instanceof Node\Expr && $this->isOptionalContainer($targetNode, $declaration)) {
                    $target .= '.value()';
                }
                return $target . '.size()';
            }
            if ($name === 'is_bool' || $name === 'is_float') {
                $value = $this->expression($expression->args[0]->value ?? null, $declaration, $method);
                $alternative = $name === 'is_bool' ? 'bool' : 'float';
                return 'std::holds_alternative<' . $alternative . '>(' . $value . ')';
            }
            if ($name === 'intdiv') {
                $left = $this->expression($expression->args[0]->value ?? null, $declaration, $method);
                $right = $this->expression($expression->args[1]->value ?? null, $declaration, $method);
                return '(' . $left . ' / ' . $right . ')';
            }
            $this->recordUnsupported($declaration, $method ?? ['name' => 'expression', 'line' => $expression->getStartLine()], 'function call requires an adapter: ' . $name);
            return 'std::monostate{}';
        }
        if ($expression instanceof Node\Expr\Isset_) {
            $values = [];
            foreach ($expression->vars as $valueNode) {
                if ($valueNode instanceof Node\Expr\ArrayDimFetch && $valueNode->dim !== null) {
                    $containerCPPType = $this->emittedExpressionType($valueNode->var, $declaration) ?? '';
                    if (str_starts_with($containerCPPType, 'std::map<') || str_starts_with($containerCPPType, 'std::unordered_map<')) {
                        $container = $this->expression($valueNode->var, $declaration, $method);
                        $key = $this->expression($valueNode->dim, $declaration, $method);
                        $values[] = $container . '.find(' . $key . ') != ' . $container . '.end()';
                        continue;
                    }
                }
                $values[] = $this->expression($valueNode, $declaration, $method) . '.has_value()';
            }
            return count($values) === 1 ? $values[0] : '(' . implode(' && ', $values) . ')';
        }
        if ($expression instanceof Node\Expr\ArrayDimFetch) {
            $array = $this->expression($expression->var, $declaration, $method);
            if ($expression->dim === null) {
                return $array;
            }
            $dimension = $this->expression($expression->dim, $declaration, $method);
            return $array . '[' . $dimension . ']';
        }
        if ($expression instanceof Node\Expr\Ternary) {
            $condition = $this->expression($expression->cond, $declaration, $method);
            $if = $this->expression($expression->if ?? $expression->cond, $declaration, $method);
            $else = $this->expression($expression->else, $declaration, $method);
            if ($this->isNullExpression($expression->if) || $this->isNullExpression($expression->else)) {
                $value = $this->isNullExpression($expression->if) ? $else : $if;
                return '([&]() -> std::optional<std::remove_cvref_t<decltype(' . $value . ')>> { if (' . $condition . ') return ' . $value . '; return std::nullopt; })()';
            }
            return '(' . $condition . ' ? ' . $if . ' : ' . $else . ')';
        }
        if ($expression instanceof Node\Expr\BinaryOp) {
            if ($expression instanceof Node\Expr\BinaryOp\Coalesce) {
                $left = $this->expression($expression->left, $declaration, $method);
                $right = $this->expression($expression->right, $declaration, $method);
                if ($this->isOptionalProperty($expression->left, $declaration)) {
                    return $left . '.value_or(' . $right . ')';
                }
                return '(' . $left . ' ? ' . $left . ' : ' . $right . ')';
            }
            if (($expression instanceof Node\Expr\BinaryOp\Equal || $expression instanceof Node\Expr\BinaryOp\Identical || $expression instanceof Node\Expr\BinaryOp\NotEqual || $expression instanceof Node\Expr\BinaryOp\NotIdentical)
                && ($this->isNullExpression($expression->left) || $this->isNullExpression($expression->right))) {
                $valueNode = $this->isNullExpression($expression->left) ? $expression->right : $expression->left;
                $isOptionalLocal = $valueNode instanceof Node\Expr\Variable
                    && ($this->expressionOptionals[(string) $valueNode->name] ?? false);
                if ($this->isOptionalProperty($valueNode, $declaration) || $isOptionalLocal) {
                    $value = $this->expression($valueNode, $declaration, $method);
                    $isEqual = $expression instanceof Node\Expr\BinaryOp\Equal || $expression instanceof Node\Expr\BinaryOp\Identical;
                    return $isEqual ? '(!' . $value . ')' : $value . '.has_value()';
                }
            }
            $operator = $this->binaryOperator($expression);
            $leftExpected = $this->isPhpIntMax($expression->left)
                ? $this->emittedExpressionType($expression->right, $declaration)
                : null;
            $rightExpected = $this->isPhpIntMax($expression->right)
                ? $this->emittedExpressionType($expression->left, $declaration)
                : null;
            $left = $this->expression($expression->left, $declaration, $method, $leftExpected);
            $right = $this->expression($expression->right, $declaration, $method, $rightExpected);
            if ($expression->left instanceof Node\Expr\Assign) {
                $left = '(' . $left . ')';
            }
            if ($expression->right instanceof Node\Expr\Assign) {
                $right = '(' . $right . ')';
            }
            if ($expression->left instanceof Node\Expr\Variable
                && ($this->expressionOptionals[(string) $expression->left->name] ?? false)
                && !$this->isNullExpression($expression->right)) {
                $left .= '.value()';
            }
            if ($expression->right instanceof Node\Expr\Variable
                && ($this->expressionOptionals[(string) $expression->right->name] ?? false)
                && !$this->isNullExpression($expression->left)) {
                $right .= '.value()';
            }
            $leftEnum = $this->enumExpressionType($expression->left, $declaration);
            $rightEnum = $this->enumExpressionType($expression->right, $declaration);
            if ($leftEnum !== null && $rightEnum === null && $expression->right instanceof Node\Expr\ClassConstFetch) {
                $right = $this->enumStorageValue($right, $leftEnum);
            }
            if ($rightEnum !== null && $leftEnum === null && $expression->left instanceof Node\Expr\ClassConstFetch) {
                $left = $this->enumStorageValue($left, $rightEnum);
            }
            if ($rightEnum !== null && $leftEnum === null && $expression->right instanceof Node\Expr\ClassConstFetch) {
                $right = $this->enumStorageValue($right, $rightEnum);
            } elseif ($leftEnum !== null && $rightEnum === null && $expression->left instanceof Node\Expr\ClassConstFetch) {
                $left = $this->enumStorageValue($left, $leftEnum);
            } elseif ($leftEnum !== null && $rightEnum === null) {
                $left = $this->enumStorageValue($left, $leftEnum);
            } elseif ($rightEnum !== null && $leftEnum === null) {
                $right = $this->enumStorageValue($right, $rightEnum);
            }
            return '(' . $left . ' ' . $operator . ' ' . $right . ')';
        }
        if ($expression instanceof Node\Expr\BooleanNot || $expression instanceof Node\Expr\UnaryMinus || $expression instanceof Node\Expr\UnaryPlus || $expression instanceof Node\Expr\BitwiseNot) {
            $operator = $expression instanceof Node\Expr\BooleanNot ? '!' : ($expression instanceof Node\Expr\BitwiseNot ? '~' : ($expression instanceof Node\Expr\UnaryMinus ? '-' : '+'));
            return $operator . $this->expression($expression->expr, $declaration, $method);
        }
        if ($expression instanceof Node\Expr\Cast) {
            $type = match (true) {
                $expression instanceof Node\Expr\Cast\Int_ => 'int32_t',
                $expression instanceof Node\Expr\Cast\Double => 'double',
                $expression instanceof Node\Expr\Cast\Bool_ => 'bool',
                $expression instanceof Node\Expr\Cast\String_ => 'std::string',
                default => 'std::monostate',
            };
            return 'static_cast<' . $type . '>(' . $this->expression($expression->expr, $declaration, $method) . ')';
        }
        if ($expression instanceof Node\Expr\BinaryOp\Coalesce) {
            $left = $this->expression($expression->left, $declaration, $method);
            $right = $this->expression($expression->right, $declaration, $method);
            if ($this->isOptionalProperty($expression->left, $declaration)) {
                return $left . '.value_or(' . $right . ')';
            }
            return '(' . $left . ' ? ' . $left . ' : ' . $right . ')';
        }
        if ($expression instanceof Node\Expr\Closure || $expression instanceof Node\Expr\ArrowFunction) {
            return $this->closure($expression, $declaration, $method);
        }
        if ($expression instanceof Node\Expr\Assign) {
            if ($expression->var instanceof Node\Expr\ArrayDimFetch
                && ($expression->var->dim === null || $this->isVectorExpression($expression->var->var, $declaration))) {
                $targetNode = $expression->var->var;
                $target = $this->expression($targetNode, $declaration, $method);
                if ($this->isOptionalContainer($targetNode, $declaration)) {
                    $target .= '.value()';
                }
                return $target
                    . '.push_back('
                    . $this->expression($expression->expr, $declaration, $method, $this->assignmentCPPType($expression->var, $declaration))
                    . ')';
            }
            $value = $this->expression($expression->expr, $declaration, $method, $this->assignmentCPPType($expression->var, $declaration));
            $enumType = $this->enumPropertyType($expression->var, $declaration)
                ?? $this->assignmentCPPType($expression->var, $declaration);
            $isEnumTarget = in_array($enumType, ['BuildPlatform', 'Type', 'Category'], true)
                || ($enumType !== null
                    && str_contains($enumType, '::')
                    && !str_starts_with($enumType, 'std::')
                    && !str_starts_with($enumType, 'glm::'))
                || ($enumType !== null && $this->isEnumLikeDeclaration($this->findDeclaration(CPPName::type($enumType)) ?? []));
            if (!$isEnumTarget) {
                $enumType = null;
            }
            if ($enumType !== null) {
                $value = $this->isStringEnumProperty($expression->var, $declaration)
                    ? $this->stringEnumFromString($expression->var, $value, $declaration)
                    : 'static_cast<' . $enumType . '>(' . $value . ')';
            }
            return $this->expression($expression->var, $declaration, $method) . ' = ' . $value;
        }
        if ($expression instanceof Node\Expr\AssignOp) {
            return $this->expression($expression->var, $declaration, $method) . ' = ' . $this->expression($expression->var, $declaration, $method) . ' ' . $this->assignOperator($expression) . ' ' . $this->expression($expression->expr, $declaration, $method);
        }
        if ($expression instanceof Node\Expr\PreInc || $expression instanceof Node\Expr\PreDec || $expression instanceof Node\Expr\PostInc || $expression instanceof Node\Expr\PostDec) {
            $operator = $expression instanceof Node\Expr\PreInc || $expression instanceof Node\Expr\PostInc ? '++' : '--';
            $value = $this->expression($expression->var, $declaration, $method);
            return $expression instanceof Node\Expr\PreInc || $expression instanceof Node\Expr\PreDec ? $operator . $value : $value . $operator;
        }
        if ($expression instanceof Node\Expr\Match_) {
            $subjectType = $this->enumPropertyType($expression->cond, $declaration)
                ?? $this->enumExpressionType($expression->cond, $declaration)
                ?? $this->expressionType($expression->cond, $declaration);
            if ($expression->cond instanceof Node\Expr\Assign) {
                $subjectType = $this->enumPropertyType($expression->cond->expr, $declaration)
                    ?? $this->enumExpressionType($expression->cond->expr, $declaration)
                    ?? $this->expressionType($expression->cond->expr, $declaration);
                if ($subjectType !== null
                    && $expression->cond->var instanceof Node\Expr\Variable
                    && $expression->cond->var->name !== 'this') {
                    $this->expressionTypes[(string) $expression->cond->var->name] = $subjectType;
                }
            }
            $subject = $this->expression($expression->cond, $declaration, $method);
            $subjectSetup = [];
            if ($expression->cond instanceof Node\Expr\Assign
                && $expression->cond->var instanceof Node\Expr\Variable
                && $expression->cond->var->name !== 'this') {
                $subjectName = $this->variableName((string) $expression->cond->var->name, $declaration);
                $subjectSetup[] = 'auto ' . $subjectName . ' = ' . $this->expression($expression->cond->expr, $declaration, $method) . ';';
                $subject = $subjectName;
            } else {
                $subjectName = null;
            }
            $arms = [];
            $default = '{}';
            $defaultIsThrow = false;
            foreach ($expression->arms as $arm) {
                $result = $this->expression($arm->body, $declaration, $method, $expectedType);
                $isThrow = str_starts_with(ltrim($result), 'throw ');
                if ($arm->conds === null) {
                    $default = $result;
                    $defaultIsThrow = $isThrow;
                    continue;
                }
                foreach ($arm->conds as $condition) {
                    $conditionValue = $this->expression($condition, $declaration, $method);
                    $conditionEnum = $this->enumExpressionType($condition, $declaration);
                    if ($conditionEnum !== null
                        && $condition instanceof Node\Expr\ClassConstFetch
                        && ($subjectType === null || CPPName::type($subjectType) !== CPPName::type($conditionEnum))) {
                        $conditionValue = $this->enumStorageValue($conditionValue, $conditionEnum);
                    }
                    $arms[] = $isThrow
                        ? 'if (' . $subject . ' == ' . $conditionValue . ') { ' . $result . ' }'
                        : 'if (' . $subject . ' == ' . $conditionValue . ') return ' . $result . ';';
                }
            }
            $lines = array_merge($subjectSetup, $arms, [$defaultIsThrow ? $default : 'return ' . $default . ';']);
            $returnType = $expectedType !== null && $expectedType !== '' && $expectedType !== 'std::monostate'
                ? ' -> ' . $expectedType
                : '';
            return "([&]()" . $returnType . " {\n" . $this->indentLines($lines, 1) . "\n}())";
        }

        $this->recordUnsupported($declaration, $method ?? ['name' => 'expression', 'line' => $expression->getStartLine()], 'expression ' . $expression::class);
        return 'std::monostate{}';
    }

private function isPhpIntMax(?Node $expression): bool
    {
        return $expression instanceof Node\Expr\ConstFetch
            && strtolower($expression->name->toString()) === 'php_int_max';
    }

private function hasInheritedProperty(array $declaration, string $propertyName): bool
    {
        $parents = array_filter(
            array_merge(
                [$declaration['parent'] ?? null],
                is_array($declaration['implements'] ?? null) ? $declaration['implements'] : [],
            ),
            static fn (mixed $parent): bool => is_string($parent) && $parent !== '',
        );
        foreach ($parents as $parent) {
            $parentDeclaration = $this->findDeclaration(CPPName::type($parent));
            foreach ($parentDeclaration['properties'] ?? [] as $property) {
                if (($property['name'] ?? null) === $propertyName) {
                    return true;
                }
            }
            if ($parentDeclaration !== null && $this->hasInheritedProperty($parentDeclaration, $propertyName)) {
                return true;
            }
        }
        return false;
    }

private function throwExpression(Node\Expr\Throw_ $throw, array $declaration, ?array $method): string
    {
        if ($throw->expr instanceof Node\Expr\StaticCall
            && $throw->expr->name instanceof Node\Identifier
            && strtolower($throw->expr->name->toString()) === 'wrap') {
            return 'throw;';
        }

        return 'throw std::exception{};';
    }
}
