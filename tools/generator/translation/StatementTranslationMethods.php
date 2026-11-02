<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\translation;

use PhpParser\Node;
use kodiak\bedrockprotocol\generator\CPPName;

trait StatementTranslationMethods
{
private function statement(Node $statement, array $declaration, array $method, int $indent, bool $returnShared = false): string
    {
        $prefix = str_repeat("\t", $indent);
        if ($statement instanceof Node\Stmt\Expression) {
            if ($statement->expr instanceof Node\Expr\Assign
                && ($statement->expr->var instanceof Node\Expr\Array_ || $statement->expr->var instanceof Node\Expr\List_)
                && ($statement->expr->expr instanceof Node\Expr\Array_ || $statement->expr->expr instanceof Node\Expr\Match_)) {
                $destructured = $this->destructuringAssignment($statement->expr, $declaration, $method);
                if ($destructured !== null) {
                    return $prefix . str_replace("\n", "\n" . $prefix, $destructured);
                }
            }
            if ($statement->expr instanceof Node\Expr\Throw_) {
                return $prefix . $this->throwExpression($statement->expr, $declaration, $method);
            }
            if ($statement->expr instanceof Node\Expr\Assign
                && $statement->expr->var instanceof Node\Expr\PropertyFetch
                && $statement->expr->expr instanceof Node\Expr\Array_
                && $statement->expr->expr->items === []
                && $this->isVectorExpression($statement->expr->var, $declaration)) {
                $target = $this->expression($statement->expr->var, $declaration, $method);
                if ($this->isOptionalContainer($statement->expr->var, $declaration)) {
                    $target .= '.value()';
                }
                return '';
            }
            if ($statement->expr instanceof Node\Expr\Assign) {
                $inlinedAppend = $this->inlineObjectAppend($statement->expr, $declaration, $method, $indent);
                if ($inlinedAppend !== null) {
                    return $inlinedAppend;
                }
                $directRead = $this->directReadAssignment($statement->expr, $declaration, $method);
                if ($directRead !== null) {
                    if ($statement->expr->var instanceof Node\Expr\Variable
                        && $statement->expr->var->name !== 'this') {
                        $name = (string) $statement->expr->var->name;
                        $this->declaredVariables[$name] = true;
                        $inferredType = $this->inferredCPPType($statement->expr->expr, $declaration);
                        if ($inferredType !== null) {
                            $this->expressionTypes[$name] = $inferredType;
                        }
                    }
                    return $prefix . str_replace("\n", "\n" . $prefix, $directRead);
                }
                $matchAssignment = $this->matchAssignment($statement->expr, $declaration, $method);
                if ($matchAssignment !== null) {
                    return $prefix . str_replace("\n", "\n" . $prefix, $matchAssignment);
                }
                $optionalAssignment = $this->optionalTernaryAssignment($statement->expr, $declaration, $method);
                if ($optionalAssignment !== null) {
                    return $prefix . str_replace("\n", "\n" . $prefix, $optionalAssignment);
                }
                $inlinedAssignment = $this->inlineMethodAssignment($statement->expr, $declaration, $method);
                if ($inlinedAssignment !== null) {
                    return $prefix . str_replace("\n", "\n" . $prefix, $inlinedAssignment);
                }
            }
            if ($statement->expr instanceof Node\Expr\Assign && $statement->expr->var instanceof Node\Expr\Variable && $statement->expr->var->name !== 'this') {
                $assigned = $statement->expr->expr;
                if ($assigned instanceof Node\Expr\New_ && $assigned->class instanceof Node\Name) {
                    $this->expressionTypes[(string) $statement->expr->var->name] = $assigned->class->getLast();
                } else {
                    $inferredType = $this->classConstantType($assigned);
                    if ($inferredType === null
                        && $assigned instanceof Node\Expr\StaticCall
                        && $assigned->class instanceof Node\Name
                        && $assigned->name instanceof Node\Identifier
                        && in_array(strtolower($assigned->name->toString()), ['frompacket', 'fromordinal'], true)) {
                        $inferredType = $assigned->class->getLast();
                    }
                    if ($inferredType !== null) {
                        $this->expressionTypes[(string) $statement->expr->var->name] = $inferredType;
                    }
                }
                $variableName = (string) $statement->expr->var->name;
                $isNullAssignment = $assigned instanceof Node\Expr\ConstFetch
                    && strtolower($assigned->name->toString()) === 'null';
                $this->expressionOptionals[$variableName] = ($this->expressionOptionals[$variableName] ?? false)
                    || $this->isOptionalExpression($assigned)
                    || $isNullAssignment;
                $value = $assigned instanceof Node\Expr\Array_
                    ? $this->emptyArrayValue((string) $statement->expr->var->name)
                    : $this->expression($assigned, $declaration, $method, $this->cppTypeForInferredVariable($assigned, $declaration));
                $assignedType = $this->inferredCPPType($assigned, $declaration);
                if (isset($this->declaredVariables[(string) $statement->expr->var->name])) {
                    if ($isNullAssignment) {
                        $value = 'std::nullopt';
                    }
                    $existingType = $this->expressionTypes[(string) $statement->expr->var->name] ?? null;
                    if (!$isNullAssignment && $existingType !== null && $assignedType !== null && $existingType !== $assignedType) {
                        return $prefix . '(void)(' . $value . ');';
                    }
                    return $prefix . $this->variableName((string) $statement->expr->var->name, $declaration)
                        . ' = ' . str_replace("\n", "\n" . $prefix, $value) . ';';
                }
                $this->declaredVariables[(string) $statement->expr->var->name] = true;
                $trackedType = $assigned instanceof Node\Expr\New_ && $assigned->class instanceof Node\Name
                    ? $assigned->class->getLast()
                    : $assignedType;
                if ($trackedType !== null) {
                    $this->expressionTypes[(string) $statement->expr->var->name] = $trackedType;
                }
                if ($isNullAssignment) {
                    return $prefix . 'std::optional<std::size_t> ' . $this->variableName((string) $statement->expr->var->name, $declaration) . ';';
                }
                return $prefix . 'auto ' . $this->variableName((string) $statement->expr->var->name, $declaration) . ' = ' . str_replace("\n", "\n" . $prefix, $value) . ';';
            }
            if ($statement->expr instanceof Node\Expr\MethodCall) {
                $inlinedStatement = $this->inlineVoidMethodStatement(
                    $statement->expr,
                    $declaration,
                    $method,
                    $indent,
                );
                if ($inlinedStatement !== null) {
                    return $inlinedStatement;
                }
            }
            $expression = $this->expression($statement->expr, $declaration, $method);
            return $prefix . str_replace("\n", "\n" . $prefix, $expression) . ';';
        }
        if ($statement instanceof Node\Stmt\Return_) {
            if (strtolower((string) ($method['returnType'] ?? '')) === 'void'
                || (!($method['static'] ?? false)
                    && in_array(strtolower((string) ($method['name'] ?? '')), ['read', 'decode'], true))) {
                return $prefix . 'return;';
            }
            if ($statement->expr instanceof Node\Expr\New_) {
                $className = $statement->expr->class instanceof Node\Name ? $statement->expr->class->getLast() : '';
                $constructed = $className === 'self' || $className === 'static'
                    ? $declaration
                    : $this->findDeclaration($className);
                if ($constructed !== null && ($returnShared || $className === $declaration['name'])) {
                    $name = CPPName::type((string) $constructed['name']);
                    $prefix = $returnShared
                        ? $prefix . 'auto result = std::make_shared<' . $name . '>();'
                        : $prefix . $name . ' result;';
                    $lines = [$prefix];
                    $constructorParameters = [];
                    foreach ($constructed['methods'] ?? [] as $candidate) {
                        if (($candidate['name'] ?? null) !== '__construct') {
                            continue;
                        }
                        foreach ($candidate['parameters'] ?? [] as $parameter) {
                            $constructorParameters[] = (string) ($parameter['name'] ?? '');
                        }
                        break;
                    }
                    $constructorProperties = [];
                    foreach ($constructed['properties'] as $property) {
                        if (!($property['static'] ?? false)) {
                            $constructorProperties[(string) ($property['name'] ?? '')] = $property;
                        }
                    }
                    if ($returnShared
                        && count($constructorParameters) === count($statement->expr->args)
                        && $constructorParameters !== []) {
                        $arguments = [];
                        foreach ($statement->expr->args as $argument) {
                            if (!$argument instanceof Node\Arg) {
                                $arguments = [];
                                break;
                            }
                            $arguments[] = $this->expression($argument->value, $declaration, $method);
                        }
                        if ($arguments !== []) {
                            return str_repeat("\t", $indent)
                                . 'return std::make_shared<' . $name . '>(' . implode(', ', $arguments) . ');';
                        }
                    }
                    $fieldIndex = 0;
                    foreach ($constructed['properties'] as $property) {
                        if ($property['static']) {
                            continue;
                        }
                        $argument = $statement->expr->args[$fieldIndex] ?? null;
                        if ($argument instanceof Node\Arg) {
                            $propertyName = $constructorParameters[$fieldIndex] ?? (string) $property['name'];
                            if (isset($constructorProperties[$propertyName])) {
                                $property = $constructorProperties[$propertyName];
                            }
                            $target = $returnShared ? 'result->' : 'result.';
                            $lines[] = $target . CPPName::identifier($property['name']) . ' = ' . $this->expression($argument->value, $declaration, $method) . ';';
                        }
                        ++$fieldIndex;
                    }
                    for ($parameterIndex = 0; $parameterIndex < count($constructorParameters); ++$parameterIndex) {
                        $parameterName = $constructorParameters[$parameterIndex];
                        if ($parameterName === '' || isset($constructorProperties[$parameterName])) {
                            continue;
                        }
                        $argument = $statement->expr->args[$parameterIndex] ?? null;
                        if (!$argument instanceof Node\Arg || !$this->hasInheritedProperty($constructed, $parameterName)) {
                            continue;
                        }
                        $target = $returnShared ? 'result->' : 'result.';
                        $lines[] = $target . CPPName::identifier($parameterName) . ' = ' . $this->expression($argument->value, $declaration, $method) . ';';
                    }
                    $lines[] = 'return result;';
                    $prefixText = array_shift($lines);
                    return $prefixText . "\n" . $this->indentLines($lines, $indent);
                }
            }
            $expression = $this->expression($statement->expr, $declaration, $method);
            return $prefix . 'return ' . str_replace("\n", "\n" . $prefix, $expression) . ';';
        }
        if ($statement instanceof Node\Stmt\Nop || $statement instanceof Node\Stmt\Declare_) {
            return '';
        }
        if ($statement instanceof Node\Stmt\Property || $statement instanceof Node\Stmt\ClassConst) {
            return '';
        }
        if ($statement instanceof Node\Stmt\Expression) {
            return $prefix . $this->expression($statement->expr, $declaration, $method) . ';';
        }
        if ($statement instanceof Node\Stmt\If_) {
            $previousDeclaredVariables = $this->declaredVariables;
            $text = $prefix . 'if (' . $this->conditionExpression($statement->cond, $declaration, $method) . ") {\n";
            $this->arrayScopes[] = $this->inferArrayScope($statement->stmts, $declaration);
            foreach ($statement->stmts as $child) {
                $text .= $this->statement($child, $declaration, $method, $indent + 1) . "\n";
            }
            array_pop($this->arrayScopes);
            $this->declaredVariables = $previousDeclaredVariables;
            $text .= $prefix . '}';
            foreach ($statement->elseifs as $elseif) {
                $text .= ' else if (' . $this->conditionExpression($elseif->cond, $declaration, $method) . ") {\n";
                $this->declaredVariables = $previousDeclaredVariables;
                $this->arrayScopes[] = $this->inferArrayScope($elseif->stmts, $declaration);
                foreach ($elseif->stmts as $child) {
                    $text .= $this->statement($child, $declaration, $method, $indent + 1) . "\n";
                }
                array_pop($this->arrayScopes);
                $this->declaredVariables = $previousDeclaredVariables;
                $text .= $prefix . '}';
            }
            if ($statement->else !== null) {
                $text .= " else {\n";
                $this->declaredVariables = $previousDeclaredVariables;
                $this->arrayScopes[] = $this->inferArrayScope($statement->else->stmts, $declaration);
                foreach ($statement->else->stmts as $child) {
                    $text .= $this->statement($child, $declaration, $method, $indent + 1) . "\n";
                }
                array_pop($this->arrayScopes);
                $this->declaredVariables = $previousDeclaredVariables;
                $text .= $prefix . '}';
            }
            return $text;
        }
        if ($statement instanceof Node\Stmt\Foreach_) {
            $previousLocalVariables = $this->localVariables;
            $previousDeclaredVariables = $this->declaredVariables;
            $elementType = $this->foreachElementType($statement->expr, $declaration);
            if ($statement->valueVar instanceof Node\Expr\Variable && $elementType !== null) {
                $this->expressionTypes[(string) $statement->valueVar->name] = $elementType;
                $this->expressionOptionals[(string) $statement->valueVar->name] = $this->nullableType($elementType);
            }
            $collection = $this->expression($statement->expr, $declaration, $method);
            if ($this->isOptionalContainer($statement->expr, $declaration)) {
                $collection .= '.value()';
            }
            if ($statement->valueVar instanceof Node\Expr\Variable) {
                $this->localVariables[(string) $statement->valueVar->name] = true;
            }
            if ($statement->keyVar instanceof Node\Expr\Variable) {
                $this->localVariables[(string) $statement->keyVar->name] = true;
            }
            $value = $this->expression($statement->valueVar, $declaration, $method);
            $key = $statement->keyVar === null ? null : $this->expression($statement->keyVar, $declaration, $method);
            $collectionCPPType = $this->emittedExpressionType($statement->expr, $declaration) ?? '';
            $isMap = str_starts_with($collectionCPPType, 'std::map<') || str_starts_with($collectionCPPType, 'std::unordered_map<');
            if ($key !== null && $isMap) {
                $text = $prefix . 'for (const auto& [' . $key . ', ' . $value . '] : ' . $collection . ") {\n";
            } else {
                $text = $prefix . 'for (const auto& ' . ($key === null ? $value : $key . 'Value') . ' : ' . $collection . ") {\n";
                if ($key !== null) {
                    $text .= $prefix . '    const auto& ' . $value . ' = ' . $key . 'Value' . ";\n";
                }
            }
            foreach ($statement->stmts as $child) {
                $text .= $this->statement($child, $declaration, $method, $indent + 1) . "\n";
            }
            $this->localVariables = $previousLocalVariables;
            $this->declaredVariables = $previousDeclaredVariables;
            return $text . $prefix . '}';
        }
        if ($statement instanceof Node\Stmt\While_) {
            $previousDeclaredVariables = $this->declaredVariables;
            $text = $prefix . 'while (' . $this->conditionExpression($statement->cond, $declaration, $method) . ") {\n";
            foreach ($statement->stmts as $child) {
                $text .= $this->statement($child, $declaration, $method, $indent + 1) . "\n";
            }
            $this->declaredVariables = $previousDeclaredVariables;
            return $text . $prefix . '}';
        }
        if ($statement instanceof Node\Stmt\For_) {
            $previousDeclaredVariables = $this->declaredVariables;
            $list = $this->listLoop($statement, $declaration, $method);
            if ($list !== null) {
                $this->declaredVariables = $previousDeclaredVariables;
                return $prefix . str_replace("\n", "\n" . $prefix, $list);
            }
            [$init, $prefixLines] = $this->forInitializer($statement->init, $declaration, $method);
            $condition = implode(' && ', array_map(fn (Node\Expr $expr): string => $this->expression($expr, $declaration, $method), $statement->cond));
            $loop = implode(', ', array_map(fn (Node\Expr $expr): string => $this->expression($expr, $declaration, $method), $statement->loop));
            $hasPrefixScope = $prefixLines !== [];
            $text = $hasPrefixScope ? $prefix . "{\n" : '';
            foreach ($prefixLines as $line) {
                $text .= $prefix . "    " . $line . "\n";
            }
            $text .= $prefix . ($hasPrefixScope ? '    ' : '') . 'for (' . $init . '; ' . $condition . '; ' . $loop . ") {\n";
            foreach ($statement->stmts as $child) {
                $text .= $this->statement($child, $declaration, $method, $indent + ($hasPrefixScope ? 2 : 1)) . "\n";
            }
            $text .= $prefix . ($hasPrefixScope ? '    ' : '') . '}';
            $this->declaredVariables = $previousDeclaredVariables;
            return $hasPrefixScope ? $text . "\n" . $prefix . '}' : $text;
        }
        if ($statement instanceof Node\Stmt\TryCatch) {
            $text = $prefix . "try {\n";
            foreach ($statement->stmts as $child) {
                $translated = $this->statement($child, $declaration, $method, $indent + 1);
                if ($translated !== '') {
                    $text .= $translated . "\n";
                }
            }
            $text .= $prefix . '}';
            foreach ($statement->catches as $catch) {
                $variable = $catch->var?->name;
                $text .= ' catch (const std::exception& ' . ($variable === null ? 'exception' : CPPName::identifier((string) $variable)) . ") {\n";
                foreach ($catch->stmts as $child) {
                    $translated = $this->statement($child, $declaration, $method, $indent + 1);
                    if ($translated !== '') {
                        $text .= $translated . "\n";
                    }
                }
                $text .= $prefix . '}';
            }
            if ($statement->finally !== null) {
                $text .= " finally {\n";
                foreach ($statement->finally->stmts as $child) {
                    $translated = $this->statement($child, $declaration, $method, $indent + 1);
                    if ($translated !== '') {
                        $text .= $translated . "\n";
                    }
                }
                $text .= $prefix . '}';
            }
            return $text;
        }
        if ($statement instanceof Node\Stmt\Throw_) {
            return $prefix . $this->throwExpression($statement->expr, $declaration, $method);
        }
        if ($statement instanceof Node\Stmt\Break_ || $statement instanceof Node\Stmt\Continue_) {
            return $prefix . ($statement instanceof Node\Stmt\Break_ ? 'break' : 'continue') . ';';
        }
        if ($statement instanceof Node\Stmt\Switch_) {
            $switchValue = $this->expression($statement->cond, $declaration, $method);
            $switchType = $this->enumPropertyType($statement->cond, $declaration) ?? $this->expressionType($statement->cond, $declaration);
            $switchCPPType = $switchType === null ? null : CPPName::type($switchType);
            $text = $prefix . 'switch (' . $switchValue . ") {\n";
            $casePrefix = str_repeat("\t", $indent + 1);
            $caseCount = count($statement->cases);
            for ($index = 0; $index < $caseCount;) {
                $labels = [];
                $body = [];
                do {
                    $case = $statement->cases[$index];
                    if ($case->cond === null) {
                        $labels[] = 'default:';
                    } else {
                        $caseValue = $this->expression($case->cond, $declaration, $method);
                        $caseEnumType = $this->enumExpressionType($case->cond, $declaration);
                        if ($caseEnumType !== null && $switchCPPType !== $caseEnumType) {
                            $caseValue = $this->enumStorageValue($caseValue, $caseEnumType);
                        }
                        $labels[] = 'case ' . $caseValue . ':';
                    }
                    foreach ($case->stmts as $child) {
                        $translated = $this->statement($child, $declaration, $method, $indent + 2);
                        if ($translated !== '') {
                            $body[] = $translated;
                        }
                    }
                    ++$index;
                } while ($body === [] && $index < $caseCount);

                if ($body === []) {
                    $text .= $casePrefix . implode("\n" . $casePrefix, $labels) . "\n";
                    continue;
                }

                if (count($labels) > 1) {
                    $text .= $casePrefix . implode("\n" . $casePrefix, array_slice($labels, 0, -1)) . "\n";
                }
                $text .= $casePrefix . end($labels) . " {\n";
                $text .= implode("\n", $body) . "\n";
                $text .= $casePrefix . "}\n";
            }
            return $text . $prefix . '}';
        }
        $this->recordUnsupported($declaration, $method, 'statement ' . $statement::class);
        return $prefix . '(void)stream;';
    }

private function optionalTernaryAssignment(
        Node\Expr\Assign $assignment,
        array $declaration,
        ?array $method,
    ): ?string {
        if (!$assignment->expr instanceof Node\Expr\Ternary
            || (!$this->isOptionalProperty($assignment->var, $declaration)
                && !($assignment->var instanceof Node\Expr\Variable
                    && ($this->expressionOptionals[(string) $assignment->var->name] ?? false)))) {
            return null;
        }
        $ternary = $assignment->expr;
        $ifIsNull = $this->isNullExpression($ternary->if);
        $elseIsNull = $this->isNullExpression($ternary->else);
        if ($ifIsNull === $elseIsNull) {
            return null;
        }
        $condition = $this->conditionExpression($ternary->cond, $declaration, $method);
        $value = $this->expression(
            $ifIsNull ? $ternary->else : ($ternary->if ?? $ternary->cond),
            $declaration,
            $method,
        );
        $target = $this->expression($assignment->var, $declaration, $method);
        $readCondition = $ifIsNull ? $this->negateCondition($condition) : $condition;
        return 'if (' . $readCondition . ") {\n"
            . "\t" . $target . ' = ' . $value . ";\n"
            . '}';
    }

private function negateCondition(string $condition): string
    {
        return preg_match('/^[A-Za-z_]\\w*(?:[.\\-]\\w+)*$/', $condition) === 1
            ? '!' . $condition
            : '!(' . $condition . ')';
    }

private function inlineObjectAppend(
        Node\Expr\Assign $assignment,
        array $declaration,
        ?array $method,
        int $indent,
    ): ?string {
        if (!$assignment->var instanceof Node\Expr\ArrayDimFetch
            || $assignment->var->dim !== null
            || !$assignment->expr instanceof Node\Expr\MethodCall
            || !$assignment->expr->var instanceof Node\Expr\Variable
            || $assignment->expr->var->name !== 'this') {
            return null;
        }

        $target = $this->expression($assignment->var->var, $declaration, $method);
        $translated = $this->withInlinedMethodContext(
            $assignment->expr,
            $declaration,
            $method,
            function (array $helper) use ($declaration, $method, $indent, $target): ?string {
                $returnType = (string) ($helper['returnType'] ?? 'void');
                if ($returnType === '' || $returnType === 'void') {
                    return null;
                }
                $body = array_values(array_filter(
                    $helper['body'] ?? [],
                    static fn (mixed $statement): bool => $statement instanceof Node\Stmt,
                ));
                $return = array_pop($body);
                if (!$return instanceof Node\Stmt\Return_ || $return->expr === null) {
                    return null;
                }
                $lines = [];
                foreach ($body as $statement) {
                    $translated = $this->statement($statement, $declaration, $helper, $indent);
                    if ($translated !== '') {
                        $lines[] = $translated;
                    }
                }
                $value = $this->expression($return->expr, $declaration, $helper);
                $lines[] = str_repeat("\t", $indent) . $target . '.push_back(' . $value . ');';
                return implode("\n", $lines);
            },
        );
        return is_string($translated) ? $translated : null;
    }

private function matchAssignment(
        Node\Expr\Assign $assignment,
        array $declaration,
        ?array $method,
    ): ?string {
        if (!$assignment->expr instanceof Node\Expr\Match_) {
            return null;
        }

        $match = $assignment->expr;
        $target = $this->expression($assignment->var, $declaration, $method);
        $targetType = $this->classConstantType($match)
            ?? $this->assignmentCPPType($assignment->var, $declaration);
        $localName = $assignment->var instanceof Node\Expr\Variable
            && $assignment->var->name !== 'this'
            ? (string) $assignment->var->name
            : null;
        if ($targetType === null) {
            foreach ($match->arms as $arm) {
                if ($arm->body instanceof Node\Expr\Throw_) {
                    continue;
                }
                $targetType = $this->classConstantType($arm->body)
                    ?? $this->inferredCPPType($arm->body, $declaration);
                if ($targetType !== null) {
                    break;
                }
            }
        }
        if ($targetType === null) {
            return null;
        }
        if ($targetType === 'bool' && $this->isBooleanMatch($match)) {
            $subject = $match->cond instanceof Node\Expr\Assign
                ? $match->cond->expr
                : $match->cond;
            return $target . ' = ' . $this->expression($subject, $declaration, $method) . ' != 0;';
        }

        $matchEnumType = $this->matchEnumType($match, $declaration);
        $subjectType = $this->enumPropertyType($match->cond, $declaration)
            ?? $this->enumExpressionType($match->cond, $declaration)
            ?? $this->expressionType($match->cond, $declaration);
        $subjectSetup = [];
        if ($match->cond instanceof Node\Expr\Assign
            && $match->cond->var instanceof Node\Expr\Variable
            && $match->cond->var->name !== 'this') {
            $subjectName = $this->variableName((string) $match->cond->var->name, $declaration);
            $subject = $subjectName;
            $subjectValue = $this->expression($match->cond->expr, $declaration, $method);
            if ($matchEnumType !== null && $subjectType !== $matchEnumType) {
                $subjectValue = 'static_cast<' . $matchEnumType . '>(' . $subjectValue . ')';
                $subjectType = $matchEnumType;
            }
            $subjectSetup[] = 'auto ' . $subjectName . ' = ' . $subjectValue . ';';
            $subjectType = $this->enumPropertyType($match->cond->expr, $declaration)
                ?? $this->enumExpressionType($match->cond->expr, $declaration)
                ?? $this->expressionType($match->cond->expr, $declaration)
                ?? $subjectType;
        } elseif ($this->simpleMatchSubject($match->cond)) {
            $subject = $this->expression($match->cond, $declaration, $method);
            if ($matchEnumType !== null && $subjectType !== $matchEnumType) {
                $subject = 'static_cast<' . $matchEnumType . '>(' . $subject . ')';
                $subjectType = $matchEnumType;
            }
        } else {
            $subject = 'matchSubject';
            $subjectSetup[] = 'auto ' . $subject . ' = '
                . $this->expression($match->cond, $declaration, $method) . ';';
            $subjectType = $this->enumPropertyType($match->cond, $declaration)
                ?? $this->enumExpressionType($match->cond, $declaration)
                ?? $this->expressionType($match->cond, $declaration)
                ?? $subjectType;
        }

        $lines = $subjectSetup;
        $isDeclared = $localName !== null && isset($this->declaredVariables[$localName]);
        if ($localName !== null && !$isDeclared) {
            $lines[] = 'auto ' . $target . ' = ' . $targetType . '{};';
            $this->declaredVariables[$localName] = true;
            $this->expressionTypes[$localName] = $targetType;
        }
        $lines[] = 'switch (' . $subject . ') {';
        foreach ($match->arms as $arm) {
            $labels = [];
            foreach ($arm->conds ?? [] as $condition) {
                $conditionValue = $this->expression($condition, $declaration, $method);
                $conditionEnum = $this->enumExpressionType($condition, $declaration);
                if ($conditionEnum !== null
                    && $condition instanceof Node\Expr\ClassConstFetch
                    && ($subjectType === null || CPPName::type($subjectType) !== CPPName::type($conditionEnum))) {
                    $conditionValue = $this->enumStorageValue($conditionValue, $conditionEnum);
                }
                $labels[] = 'case ' . $conditionValue . ':';
            }
            $body = $this->expression($arm->body, $declaration, $method, $targetType);
            if ($arm->conds === null) {
                $labels[] = 'default:';
            }
            if ($labels === []) {
                continue;
            }
            foreach ($labels as $label) {
                $lines[] = "\t" . $label;
            }
            $lines[] = "\t{";
            if (str_starts_with(ltrim($body), 'throw ')) {
                $lines[] = "\t\t" . $body;
            } else {
                $lines[] = "\t\t" . $target . ' = ' . $body . ';';
                $lines[] = "\t\tbreak;";
            }
            $lines[] = "\t}";
        }
        $lines[] = '}';
        return implode("\n", $lines);
    }

private function matchEnumType(Node\Expr\Match_ $match, array $declaration): ?string
    {
        foreach ($match->arms as $arm) {
            foreach ($arm->conds ?? [] as $condition) {
                $type = $this->enumExpressionType($condition, $declaration);
                if ($type !== null) {
                    return $type;
                }
            }
        }
        return null;
    }

private function isBooleanMatch(Node\Expr\Match_ $match): bool
    {
        $hasFalse = false;
        $hasTrue = false;
        $hasDefaultThrow = false;
        foreach ($match->arms as $arm) {
            if ($arm->conds === null) {
                $hasDefaultThrow = $arm->body instanceof Node\Expr\Throw_;
                continue;
            }
            if (count($arm->conds) !== 1 || !$arm->conds[0] instanceof Node\Scalar\Int_) {
                return false;
            }
            $value = (int) $arm->conds[0]->value;
            $isTrue = $arm->body instanceof Node\Expr\ConstFetch
                && strtolower($arm->body->name->toString()) === 'true';
            $isFalse = $arm->body instanceof Node\Expr\ConstFetch
                && strtolower($arm->body->name->toString()) === 'false';
            if ($value === 0 && $isFalse) {
                $hasFalse = true;
            } elseif ($value === 1 && $isTrue) {
                $hasTrue = true;
            } else {
                return false;
            }
        }
        return $hasFalse && $hasTrue && $hasDefaultThrow;
    }

private function simpleMatchSubject(Node\Expr $expression): bool
    {
        return $expression instanceof Node\Expr\Variable
            || $expression instanceof Node\Expr\PropertyFetch
            || $expression instanceof Node\Expr\ClassConstFetch;
    }

private function conditionExpression(Node\Expr $condition, array $declaration, ?array $method): string
    {
        $value = trim($this->expression($condition, $declaration, $method));
        while ($this->hasOuterParentheses($value)) {
            $value = trim(substr($value, 1, -1));
        }
        return $value;
    }

private function hasOuterParentheses(string $value): bool
    {
        if (strlen($value) < 2 || $value[0] !== '(' || $value[-1] !== ')') {
            return false;
        }
        $depth = 0;
        $length = strlen($value);
        for ($index = 0; $index < $length; ++$index) {
            if ($value[$index] === '(') {
                ++$depth;
            } elseif ($value[$index] === ')') {
                --$depth;
                if ($depth === 0 && $index < $length - 1) {
                    return false;
                }
            }
            if ($depth < 0) {
                return false;
            }
        }
        return $depth === 0;
    }

private function inlineMethodAssignment(
        Node\Expr\Assign $assignment,
        array $declaration,
        ?array $method,
    ): ?string {
        if (!$assignment->expr instanceof Node\Expr\MethodCall
            || !$assignment->expr->var instanceof Node\Expr\Variable
            || $assignment->expr->var->name !== 'this') {
            return null;
        }

        $target = $this->expression($assignment->var, $declaration, $method);
        $translated = $this->withInlinedMethodContext(
            $assignment->expr,
            $declaration,
            $method,
            function (array $helper) use ($declaration, $target): ?string {
                $body = array_values(array_filter(
                    $helper['body'] ?? [],
                    static fn (mixed $statement): bool => $statement instanceof Node\Stmt,
                ));
                if (count($body) !== 2
                    || !$body[0] instanceof Node\Stmt\If_
                    || !$body[1] instanceof Node\Stmt\Return_
                    || $body[0]->elseifs !== []
                    || $body[0]->else !== null
                    || count($body[0]->stmts) !== 1
                    || !$body[0]->stmts[0] instanceof Node\Stmt\Return_
                    || $body[0]->stmts[0]->expr === null
                    || $body[1]->expr === null) {
                    return null;
                }
                $condition = $this->conditionExpression($body[0]->cond, $declaration, $helper);
                $whenTrue = $this->expression($body[0]->stmts[0]->expr, $declaration, $helper);
                $whenFalse = $this->expression($body[1]->expr, $declaration, $helper);
                if ($this->isDefaultReturnExpression($body[1]->expr)) {
                    return 'if (' . $condition . ") {\n"
                        . "\t" . $target . ' = ' . $whenTrue . ";\n"
                        . '}';
                }
                return 'if (' . $condition . ") {\n"
                    . "\t" . $target . ' = ' . $whenTrue . ";\n"
                    . "} else {\n"
                    . "\t" . $target . ' = ' . $whenFalse . ";\n"
                    . '}';
            },
        );
        return is_string($translated) ? $translated : null;
    }

private function isDefaultReturnExpression(Node\Expr $expression): bool
    {
        if ($expression instanceof Node\Scalar\Int_ || $expression instanceof Node\Scalar\Float_) {
            return (float) $expression->value === 0.0;
        }
        return $expression instanceof Node\Expr\ConstFetch
            && in_array(strtolower($expression->name->toString()), ['false', 'null'], true);
    }

private function inlineVoidMethodStatement(
        Node\Expr\MethodCall $call,
        array $declaration,
        ?array $method,
        int $indent,
    ): ?string {
        if (!$call->var instanceof Node\Expr\Variable || $call->var->name !== 'this') {
            return null;
        }

        $translated = $this->withInlinedMethodContext(
            $call,
            $declaration,
            $method,
            function (array $helper) use ($declaration, $indent): ?string {
                $returnType = (string) ($helper['returnType'] ?? 'void');
                if ($returnType !== '' && $returnType !== 'void') {
                    return null;
                }
                $lines = [];
                foreach ($helper['body'] ?? [] as $statement) {
                    $translated = $this->statement($statement, $declaration, $helper, $indent);
                    if ($translated !== '') {
                        $lines[] = $translated;
                    }
                }
                return $lines === [] ? null : implode("\n", $lines);
            },
        );
        return is_string($translated) ? $translated : null;
    }

private function destructuringAssignment(Node\Expr\Assign $assignment, array $declaration, array $method): ?string
    {
        if (!$assignment->var instanceof Node\Expr\Array_ && !$assignment->var instanceof Node\Expr\List_) {
            return null;
        }
        $names = [];
        foreach ($assignment->var->items as $item) {
            if ($item === null || !$item->value instanceof Node\Expr\Variable) {
                return null;
            }
            $names[] = $this->variableName((string) $item->value->name, $declaration);
        }
        if ($names === []) {
            return null;
        }

        if (!$assignment->expr instanceof Node\Expr\Match_) {
            if (!$assignment->expr instanceof Node\Expr\Array_) {
                return null;
            }
            $lines = [];
            foreach ($assignment->expr->items as $index => $item) {
                if ($item === null || !isset($names[$index])) {
                    continue;
                }
                $lines[] = 'auto ' . $names[$index] . ' = ' . $this->expression($item->value, $declaration, $method) . ';';
            }
            return $lines === [] ? null : implode("\n", $lines);
        }

        $elementTypes = $this->matchTupleTypes($assignment->expr, $declaration);
        if (count($elementTypes) !== count($names)) {
            return null;
        }
        $tupleType = 'std::tuple<' . implode(', ', $elementTypes) . '>';
        $value = $this->expression($assignment->expr, $declaration, $method, $tupleType);
        return 'auto [' . implode(', ', $names) . '] = ' . $value . ';';
    }

private function matchTupleTypes(Node\Expr\Match_ $match, array $declaration): array
    {
        $candidates = [];
        foreach ($match->arms as $arm) {
            if (!$arm->body instanceof Node\Expr\Array_) {
                continue;
            }
            foreach ($arm->body->items as $index => $item) {
                if ($item === null) {
                    continue;
                }
                $type = $this->inferredCPPType($item->value, $declaration);
                if ($type !== null) {
                    $candidates[$index][] = $type;
                }
            }
        }

        $result = [];
        if ($candidates === []) {
            return [];
        }
        $lastIndex = max(array_keys($candidates));
        for ($index = 0; $index <= $lastIndex; ++$index) {
            $types = $candidates[$index] ?? [];
            if ($types === []) {
                return [];
            }
            $concrete = array_values(array_filter($types, static fn (string $type): bool => !str_starts_with($type, 'std::variant<')));
            if (count(array_unique($concrete)) === 1) {
                $result[] = $concrete[0];
                continue;
            }
            $alternatives = [];
            foreach ($types as $type) {
                $parts = $this->templateArguments($type, 'std::variant');
                foreach ($parts === [] ? [$type] : $parts as $part) {
                    if ($part !== 'std::monostate' && !in_array($part, $alternatives, true)) {
                        $alternatives[] = $part;
                    }
                }
            }
            if ($alternatives === []) {
                return [];
            }
            $result[] = count($alternatives) === 1
                ? $alternatives[0]
                : 'std::variant<' . implode(', ', $alternatives) . '>';
        }
        return $result;
    }
}
