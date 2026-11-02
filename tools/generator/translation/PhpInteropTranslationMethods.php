<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\translation;

use PhpParser\Node;
use kodiak\bedrockprotocol\generator\CPPName;

trait PhpInteropTranslationMethods
{
private function staticCall(Node\Expr\StaticCall $call, array $declaration, ?array $method): string
    {
        $class = $call->class instanceof Node\Name ? $call->class->getLast() : '';
        if ($class === 'self' || $class === 'static') {
            $class = CPPName::type((string) $declaration['name']);
        }
        $name = $call->name instanceof Node\Identifier ? $call->name->toString() : 'call';
        $arguments = array_map(fn (Node $argument): string => $this->argumentExpression($argument, $declaration, $method), $call->args);
        $rawArguments = array_map(fn (Node $argument): string => $this->argumentExpression($argument, $declaration, $method, false), $call->args);
        if (count($call->args) > 1
            && (str_starts_with(strtolower($name), 'write') || str_starts_with(strtolower($name), 'put'))) {
            foreach (array_slice($call->args, 1, null, true) as $index => $argument) {
                if ($argument instanceof Node\Arg && !$this->isStringEnumProperty($argument->value, $declaration)) {
                    $arguments[$index] = $this->enumStorageCast($argument, $arguments[$index], $declaration);
                }
            }
        }
        $stream = $arguments[0] ?? 'stream';
        $isReader = isset($call->args[0]) && $call->args[0] instanceof Node\Arg && $this->isReaderExpression($call->args[0]->value);
        if ($this->catalog->isSerializerClass($class)) {
            if ($name === 'readList') {
                $callback = $arguments[1] ?? '[&]() { return std::monostate{}; }';
                return $stream . '.readList(' . $callback . ')';
            }
            if ($name === 'writeList') {
                $callback = $arguments[2] ?? '[&](const auto&) {}';
                return $stream . '.writeList(' . ($arguments[1] ?? 'std::monostate{}') . ', ' . $callback . ')';
            }
            if (count($call->args) === 1 && $this->isVariadicArgument($call->args[0])) {
                $reading = str_starts_with(strtolower($name), 'get') || str_starts_with(strtolower($name), 'read');
                $mapped = $this->streamMethod($class, $name, !$reading);
                if ($mapped !== null) {
                    return $reading
                        ? $this->callback('', 'return stream.' . $mapped . '().value_or(' . $this->readDefault($mapped) . ');')
                        : $this->callback('const auto& value', 'stream.' . $mapped . '(value);');
                }
                $codec = $this->specialCodecCallback($class, $name);
                if ($codec !== null) {
                    return $codec;
                }
            }
            $mapped = $this->streamMethod($class, $name, !$isReader);
            if ($mapped !== null) {
                if (!$isReader && isset($call->args[1]) && $call->args[1] instanceof Node\Arg && $this->isUnknownProperty($call->args[1]->value, $declaration)) {
                    $this->recordUnsupported($declaration, $method ?? ['name' => 'expression', 'line' => $call->getStartLine()], 'serializer write of an unknown value');
                }
                $writeArguments = array_slice($arguments, 1);
                if (!$isReader) {
                    foreach (array_slice($call->args, 1, null, true) as $index => $argument) {
                        if ($argument instanceof Node\Arg && $this->isStringEnumProperty($argument->value, $declaration)) {
                            $writeArguments[$index - 1] = $this->stringEnumToString($argument->value, $writeArguments[$index - 1], $declaration);
                        }
                    }
                }
                $result = $stream . '.' . $mapped . '(' . implode(', ', $writeArguments) . ')';
                return $isReader ? $result . '.value_or(' . $this->readDefault($mapped) . ')' : $result;
            }
            $sourceCodec = $this->sourceCodecCall($name, $isReader, $arguments);
            if ($sourceCodec !== null) {
                return $sourceCodec;
            }
            $directCodec = $this->specialCodecExpression($class, $name, $isReader, $rawArguments);
            if ($directCodec !== null) {
                return $directCodec;
            }
            if ($name === 'readOptional') {
                $callback = $arguments[1] ?? '[&]() { return std::monostate{}; }';
                return $stream . '.readOptional(' . $callback . ')';
            }
            if ($name === 'writeOptional') {
                if (isset($call->args[1]) && $call->args[1] instanceof Node\Arg && $this->isUnknownProperty($call->args[1]->value, $declaration)) {
                    $this->recordUnsupported($declaration, $method ?? ['name' => 'expression', 'line' => $call->getStartLine()], 'optional write of an unknown value');
                }
                $optionalArguments = array_map(fn (Node $argument): string => $this->argumentExpression($argument, $declaration, $method, false), $call->args);
                $value = $optionalArguments[1] ?? 'std::monostate{}';
                $callback = $arguments[2] ?? '[&](const auto&) {}';
                return $stream . '.writeOptional(' . $value . ', ' . $callback . ')';
            }
            $this->recordUnsupported($declaration, $method ?? ['name' => 'expression', 'line' => $call->getStartLine()], 'serializer call requires an adapter: ' . $class . '::' . $name);
            return 'std::monostate{}';
        }
        $typeCodec = $this->catalog->typeCodec($class);
        if (is_array($typeCodec)) {
            if ($name === 'read' && isset($typeCodec['read']) && is_string($typeCodec['read'])) {
                if (preg_match('/^([A-Za-z_][A-Za-z0-9_]*)::read\(stream\)$/', trim($typeCodec['read']), $matches) === 1) {
                    return $this->readObject($matches[1]);
                }
                return $typeCodec['read'];
            }
            if ($name === 'write' && isset($typeCodec['write']) && is_string($typeCodec['write'])) {
                return str_replace('%VALUE%', $arguments[1] ?? 'std::monostate{}', $typeCodec['write']);
            }
        }
        if ($name === 'fromPacket' || $name === 'fromOrdinal') {
            $referenced = $this->findDeclaration($class);
            if ($referenced !== null && $referenced['kind'] === 'enum' && $referenced['backingType'] === 'string') {
                $value = $arguments[0] ?? '""';
                $enumType = CPPName::type($class);
                $default = $this->enumDefaultValue($enumType, $declaration);
                if ($default === null) {
                    throw new \RuntimeException('Unable to resolve the first case of enum ' . $enumType);
                }
                return 'magic_enum::enum_cast<' . $enumType . '>(std::string_view(' . $value . ')).value_or(' . $default . ')';
            }
            if ($referenced !== null
                && (($referenced['kind'] ?? null) === 'enum' || $this->isEnumLikeDeclaration($referenced))) {
                return $this->enumReadValue(CPPName::type($class), $arguments[0] ?? '0', $declaration);
            }
            return 'static_cast<' . CPPName::type($class) . '>(' . ($arguments[0] ?? '0') . ')';
        }
        if ($class === 'parent') {
            if (strtolower($name) === '__construct') {
                return '(void)0';
            }
            $this->recordUnsupported($declaration, $method ?? ['name' => 'expression', 'line' => $call->getStartLine()], 'parent dispatch requires an inheritance adapter');
            return 'std::monostate{}';
        }
        $staticAlias = $this->catalog->staticCallAlias($class, $name);
        if ($staticAlias !== null) {
            $staticAlias = str_replace('%VALUE%', $arguments[0] ?? '0', $staticAlias);
            foreach ($arguments as $index => $argument) {
                $staticAlias = str_replace('%ARG' . $index . '%', $argument, $staticAlias);
            }
            if (count($call->args) === 1 && $this->isVariadicArgument($call->args[0])) {
                return '[&]() { return ' . $staticAlias . '; }';
            }
            return $staticAlias;
        }
        $cppClass = $class === 'self' || $class === 'static' ? CPPName::type($declaration['name']) : CPPName::type($class);
        $referenced = $this->findDeclaration($class === 'self' || $class === 'static' ? $declaration['name'] : $class);
        if (count($call->args) === 1
            && $this->isVariadicArgument($call->args[0])
            && in_array($name, ['read', 'write'], true)) {
            return $name === 'read'
                ? '[&]() { return ' . $this->readObject($cppClass) . '; }'
                : '[&](const auto& value) { value->write(stream); }';
        }
        if (in_array($name, ['read', 'decode'], true) && $referenced !== null) {
            $hasContextStaticMethod = false;
            $hasStaticMethod = false;
            foreach ($referenced['methods'] ?? [] as $candidate) {
                if (!($candidate['static'] ?? false)
                    || strtolower((string) ($candidate['name'] ?? '')) !== strtolower($name)) {
                    continue;
                }
                $hasStaticMethod = true;
                $hasContextStaticMethod = $hasContextStaticMethod || count($candidate['parameters'] ?? []) > 1;
            }
            if ($hasContextStaticMethod) {
                return $cppClass . '::' . CPPName::identifier($name) . '(' . implode(', ', $arguments) . ')';
            }
            if ($hasStaticMethod) {
                return $this->readObject($cppClass);
            }
        }
        if (count($call->args) > 1 && in_array($name, ['read', 'write'], true)) {
            if ($this->catalog->staticCallAlias($class, $name) !== null) {
                $staticAlias = $this->catalog->staticCallAlias($class, $name);
                foreach ($arguments as $index => $argument) {
                    $staticAlias = str_replace('%ARG' . $index . '%', $argument, $staticAlias);
                }
                return str_replace('%VALUE%', $arguments[0] ?? '0', $staticAlias);
            }
            $this->recordUnsupported($declaration, $method ?? ['name' => 'expression', 'line' => $call->getStartLine()], 'contextual type codec requires an adapter: ' . $cppClass . '::' . $name);
            return 'std::monostate{}';
        }
        if ($name === 'read' && ($referenced === null || !$this->hasMethod($referenced, 'read', true))) {
            $this->recordUnsupported($declaration, $method ?? ['name' => 'expression', 'line' => $call->getStartLine()], 'static read is not emitted: ' . $cppClass);
            return 'std::monostate{}';
        }
        if (!in_array($name, ['read', 'write', 'fromPacket', 'fromOrdinal'], true)) {
            $this->recordUnsupported($declaration, $method ?? ['name' => 'expression', 'line' => $call->getStartLine()], 'static call requires an adapter: ' . $cppClass . '::' . $name);
            return 'std::monostate{}';
        }
        return $cppClass . '::' . CPPName::identifier($name) . '(' . implode(', ', $arguments) . ')';
    }

private function sourceCodecCall(string $name, bool $reader, array $arguments): ?string
    {
        $normalized = strtolower($name);
        foreach ($this->declarations as $owner) {
            if (!$this->catalog->isSerializerClass((string) ($owner['name'] ?? ''))) {
                continue;
            }
            foreach ($owner['methods'] ?? [] as $method) {
                if (!$method['static'] || strtolower((string) $method['name']) !== $normalized) {
                    continue;
                }
                if ($reader) {
                    $target = CPPName::type((string) ($method['returnType'] ?? ''));
                    $referenced = $this->findDeclaration($target);
                    if ($referenced !== null && $referenced['kind'] === 'class') {
                        return $this->readObject($target);
                    }
                    continue;
                }
                foreach (array_slice($method['parameters'] ?? [], 1) as $parameter) {
                    $target = CPPName::type((string) ($parameter['type'] ?? ''));
                    $referenced = $this->findDeclaration($target);
                    if ($referenced !== null && $referenced['kind'] === 'class') {
                        $value = $arguments[1] ?? 'std::monostate{}';
                        $cppTarget = $this->cppType($target);
                        return str_starts_with($cppTarget, 'std::shared_ptr<')
                            ? 'if (' . $value . ') ' . $value . '->write(stream)'
                            : $value . '.write(stream)';
                    }
                }
            }
        }
        return null;
    }

private function classConstant(Node\Expr\ClassConstFetch $fetch, array $declaration): string
    {
        $class = $fetch->class instanceof Node\Name ? $fetch->class->getLast() : '';
        $name = $fetch->name instanceof Node\Identifier ? $fetch->name->toString() : 'value';
        if ($class === 'self' || $class === 'static') {
            $current = $this->findDeclaration($declaration['name']) ?? $declaration;
            foreach ($current['constants'] ?? [] as $constant) {
                if ($constant['name'] === $name && is_string($constant['value'])) {
                    return 'std::string(' . CPPName::type($declaration['name']) . '::' . CPPName::identifier($name) . ')';
                }
            }
            if (str_starts_with($name, 'TYPE_')) {
                return 'Type::' . CPPName::enumCase(substr($name, 5));
            }
            if (str_starts_with($name, 'CATEGORY_')) {
                return 'Category::' . CPPName::enumCase(substr($name, 9));
            }
            $enumCase = $this->constantEnumCase($current, $name);
            if ($enumCase !== null) {
                return $enumCase;
            }
            if (str_starts_with($name, 'ACTION_') && $this->hasActionEnum($current)) {
                return 'Action::' . CPPName::enumCase(substr($name, 7));
            }
            return CPPName::identifier($name);
        }
        $referenced = $this->findDeclaration($class);
        $alias = $this->catalog->constantAlias($class, $name);
        if ($alias !== null) {
            return $alias;
        }
        if ($name === 'NETWORK_ID') {
            return 'ID';
        }
        if ($referenced !== null && str_starts_with($name, 'TYPE_')) {
            foreach ($referenced['constants'] ?? [] as $constant) {
                if (str_starts_with((string) ($constant['name'] ?? ''), 'TYPE_')) {
                    return CPPName::type($class) . '::Type::' . CPPName::enumCase(substr($name, 5));
                }
            }
        }
        if ($referenced !== null && str_starts_with($name, 'CATEGORY_')) {
            foreach ($referenced['constants'] ?? [] as $constant) {
                if (str_starts_with((string) ($constant['name'] ?? ''), 'CATEGORY_')) {
                    return CPPName::type($class) . '::Category::' . CPPName::enumCase(substr($name, 9));
                }
            }
        }
        if ($referenced !== null && str_starts_with($name, 'ACTION_') && $this->hasActionEnum($referenced)) {
            return CPPName::type($class) . '::Action::' . CPPName::enumCase(substr($name, 7));
        }
        if ($referenced !== null) {
            $enumCase = $this->constantEnumCase($referenced, $name);
            if ($enumCase !== null) {
                return CPPName::type($class) . '::' . $enumCase;
            }
        }
        if ($referenced !== null && $this->isEnumLikeDeclaration($referenced)) {
            return CPPName::type($class) . '::' . CPPName::enumCase($name);
        }
        foreach ($referenced['constants'] ?? [] as $constant) {
            if ($constant['name'] === $name && is_string($constant['value'])) {
                return 'std::string(' . CPPName::type($class) . '::' . CPPName::identifier($name) . ')';
            }
        }
        return CPPName::type($class) . '::' . CPPName::identifier($name);
    }

    private function constantEnumCase(array $declaration, string $name): ?string
    {
        if (!str_contains($name, '_')) {
            return null;
        }
        [$prefix, $case] = explode('_', $name, 2);
        $count = 0;
        foreach ($declaration['constants'] ?? [] as $constant) {
            $constantName = (string) ($constant['name'] ?? '');
            if (str_starts_with($constantName, $prefix . '_')
                && (is_int($constant['value'] ?? null) || is_bool($constant['value'] ?? null))) {
                ++$count;
            }
        }
        if ($count >= 2 || in_array(strtoupper($prefix), ['TYPE', 'CATEGORY'], true)) {
            return CPPName::enumCase($prefix) . '::' . CPPName::enumCase($case);
        }
        foreach ($declaration['properties'] ?? [] as $property) {
            if (($property['static'] ?? false)
                || ltrim((string) ($property['type'] ?? ''), '?') !== 'int') {
                continue;
            }
            $propertyName = strtolower((string) ($property['name'] ?? ''));
            if ($this->constantPrefixMatchesProperty($prefix, $propertyName)) {
                return CPPName::enumCase($prefix) . '::' . CPPName::enumCase($case);
            }
        }
        return null;
    }

    private function constantPrefixMatchesProperty(string $prefix, string $propertyName): bool
    {
        $prefix = preg_replace('/[^a-z0-9]/', '', strtolower($prefix)) ?? strtolower($prefix);
        $propertyName = preg_replace('/[^a-z0-9]/', '', strtolower($propertyName)) ?? strtolower($propertyName);
        if ($prefix === '' || $propertyName === $prefix) {
            return $prefix !== '';
        }
        $semanticNames = [
            'type',
            'action',
            'id',
            'mode',
            'state',
            'status',
            'result',
            'request',
            'event',
            'property',
            'reason',
            'severity',
            'algorithm',
            'operation',
            'value',
            'kind',
            'level',
            'order',
            'layout',
            'visibility',
            'category',
            'rotation',
            'layer',
            'layerid',
            'scheme',
            'permission',
            'permissions',
            'effect',
            'target',
        ];
        if (str_ends_with($propertyName, $prefix)) {
            return in_array($prefix, $semanticNames, true);
        }
        if (!str_starts_with($propertyName, $prefix)) {
            return false;
        }
        return in_array(substr($propertyName, strlen($prefix)), ['s', ...$semanticNames], true);
    }

    private function hasActionEnum(array $declaration): bool
    {
        $hasActionConstants = false;
        foreach ($declaration['constants'] ?? [] as $constant) {
            if (str_starts_with((string) ($constant['name'] ?? ''), 'ACTION_')) {
                $hasActionConstants = true;
                break;
            }
        }
        if (!$hasActionConstants) {
            return false;
        }
        foreach ($declaration['properties'] ?? [] as $property) {
            $name = strtolower((string) ($property['name'] ?? ''));
            if ($name === 'action'
                || str_ends_with($name, 'action')
                || str_ends_with($name, 'actiontype')) {
                return true;
            }
        }
        return false;
    }

private function closure(Node\Expr\Closure|Node\Expr\ArrowFunction $closure, array $declaration, ?array $method): string
    {
        $previousTypes = $this->expressionTypes;
        $previousAliases = $this->expressionAliases;
        $previousOptionals = $this->expressionOptionals;
        $params = [];
        foreach ($closure->params as $parameter) {
            $parameterName = (string) $parameter->var->name;
            if ($parameterName === 'out' || $parameterName === 'in') {
                continue;
            }
            $parameterType = $parameter->type?->toString() ?? '';
            if ($parameterType !== '') {
                $this->expressionTypes[$parameterName] = CPPName::type($parameterType);
            }
            $mutable = str_ends_with($parameterType, 'Writer');
            $params[] = ($mutable ? 'auto& ' : 'const auto& ') . CPPName::identifier($parameterName);
        }
        if ($closure instanceof Node\Expr\ArrowFunction) {
            $expression = $this->expression($closure->expr, $declaration, $method);
            $returnsVoid = $this->isVoidExpression($closure->expr);
            $result = $this->callback(implode(', ', $params), ($returnsVoid ? '' : 'return ') . $expression . ';');
            $this->expressionAliases = $previousAliases;
            $this->expressionTypes = $previousTypes;
            $this->expressionOptionals = $previousOptionals;
            return $result;
        }
        $body = [];
        foreach ($closure->stmts as $statement) {
            $translated = $this->statement($statement, $declaration, $method ?? ['name' => 'closure', 'line' => 0], 1);
            if ($translated !== '') {
                $body[] = $translated;
            }
        }
        $formattedBody = array_map(static function (string $line): string {
            $line = preg_replace_callback(
                '/^(?: {4})+/',
                static fn (array $matches): string => str_repeat("\t", (int) (strlen($matches[0]) / 4)),
                $line,
            ) ?? $line;
            return "\t\t" . $line;
        }, $body);
        $result = '[&](' . implode(', ', $params) . ') {';
        if ($formattedBody !== []) {
            $result .= "\n" . implode("\n", $formattedBody) . "\n\t\t}";
        } else {
            $result .= '}';
        }
        $this->expressionAliases = $previousAliases;
        $this->expressionTypes = $previousTypes;
        $this->expressionOptionals = $previousOptionals;
        return $result;
    }

private function arguments(array $arguments, array $declaration, ?array $method): string
    {
        return implode(', ', array_map(fn (Node $argument): string => $this->argumentExpression($argument, $declaration, $method), $arguments));
    }

private function argumentExpression(Node $argument, array $declaration, ?array $method, bool $dereferenceOptional = true): string
    {
        if ($argument instanceof Node\Arg) {
            $value = $this->expression($argument->value, $declaration, $method);
            if ($dereferenceOptional && ($this->isOptionalProperty($argument->value, $declaration) || $this->isOptionalExpression($argument->value))) {
                return '*(' . $value . ')';
            }
            return $value;
        }
        if ($argument instanceof Node\VariadicPlaceholder) {
            return '/* variadic callback */';
        }
        return 'std::monostate{}';
    }

private function isVariadicArgument(Node $argument): bool
    {
        return $argument instanceof Node\VariadicPlaceholder
            || ($argument instanceof Node\Arg && $argument->value instanceof Node\VariadicPlaceholder);
    }

private function specialCodecExpression(string $class, string $name, bool $reader, array $arguments): ?string
    {
        $codec = $this->catalog->specialCodec($class, $name);
        if ($codec === null) {
            return null;
        }
        $body = $reader ? ($codec['read'] ?? null) : ($codec['write'] ?? null);
        if (!is_string($body)) {
            return null;
        }
        if ($reader && preg_match('/^([A-Za-z_][A-Za-z0-9_]*)::read\(stream\)$/', trim($body), $matches) === 1) {
            return $this->readObject($matches[1]);
        }
        $body = str_replace('%VALUE%', $arguments[1] ?? 'std::monostate{}', $body);
        foreach ($arguments as $index => $argument) {
            $body = str_replace('%ARG' . $index . '%', $argument, $body);
        }
        return $body;
    }

private function specialCodecCallback(string $class, string $name): ?string
    {
        $codec = $this->catalog->specialCodec($class, $name);
        if ($codec === null) {
            return null;
        }
        if (isset($codec['read']) && is_string($codec['read'])) {
            return $this->callback('', 'return ' . $codec['read'] . ';');
        }
        if (isset($codec['write']) && is_string($codec['write'])) {
            return $this->callback('const auto& value', str_replace('%VALUE%', 'value', $codec['write']) . ';');
        }
        return null;
    }

private function streamMethod(string $class, string $name, bool $writer): ?string
    {
        return $this->catalog->streamMethod($class, $name);
    }

private function readDefault(string $method): string
    {
        if (str_contains($method, 'String') || $method === 'readUUID') {
            return '""';
        }
        if (str_contains($method, 'Float')) {
            return '0.0f';
        }
        if ($method === 'readDouble') {
            return '0.0';
        }
        if ($method === 'readBool') {
            return 'false';
        }
        if ($method === 'readVec2' || $method === 'readRotationVec2') {
            return 'glm::vec2(0.0f)';
        }
        if ($method === 'readVec3') {
            return 'glm::vec3(0.0f)';
        }
        if (str_contains($method, 'BlockPos')) {
            return 'glm::ivec3(0)';
        }
        if ($method === 'readByteArray') {
            return 'std::vector<uint8_t>()';
        }
        return '0';
    }

private function isReaderExpression(?Node $node): bool
    {
        return $node instanceof Node\Expr\Variable && in_array($node->name, ['in', 'connRequestReader'], true);
    }

private function binaryOperator(Node\Expr\BinaryOp $expression): string
    {
        return match (true) {
            $expression instanceof Node\Expr\BinaryOp\Plus => '+',
            $expression instanceof Node\Expr\BinaryOp\Minus => '-',
            $expression instanceof Node\Expr\BinaryOp\Mul => '*',
            $expression instanceof Node\Expr\BinaryOp\Div => '/',
            $expression instanceof Node\Expr\BinaryOp\Mod => '%',
            $expression instanceof Node\Expr\BinaryOp\BitwiseOr => '|',
            $expression instanceof Node\Expr\BinaryOp\BitwiseAnd => '&',
            $expression instanceof Node\Expr\BinaryOp\BitwiseXor => '^',
            $expression instanceof Node\Expr\BinaryOp\ShiftLeft => '<<',
            $expression instanceof Node\Expr\BinaryOp\ShiftRight => '>>',
            $expression instanceof Node\Expr\BinaryOp\BooleanAnd, $expression instanceof Node\Expr\BinaryOp\LogicalAnd => '&&',
            $expression instanceof Node\Expr\BinaryOp\BooleanOr, $expression instanceof Node\Expr\BinaryOp\LogicalOr => '||',
            $expression instanceof Node\Expr\BinaryOp\Equal, $expression instanceof Node\Expr\BinaryOp\Identical => '==',
            $expression instanceof Node\Expr\BinaryOp\NotEqual, $expression instanceof Node\Expr\BinaryOp\NotIdentical => '!=',
            $expression instanceof Node\Expr\BinaryOp\Smaller => '<',
            $expression instanceof Node\Expr\BinaryOp\SmallerOrEqual => '<=',
            $expression instanceof Node\Expr\BinaryOp\Greater => '>',
            $expression instanceof Node\Expr\BinaryOp\GreaterOrEqual => '>=',
            $expression instanceof Node\Expr\BinaryOp\Concat => '+',
            default => '+',
        };
    }

private function assignOperator(Node\Expr\AssignOp $expression): string
    {
        return match (true) {
            $expression instanceof Node\Expr\AssignOp\Plus => '+',
            $expression instanceof Node\Expr\AssignOp\Minus => '-',
            $expression instanceof Node\Expr\AssignOp\Mul => '*',
            $expression instanceof Node\Expr\AssignOp\Div => '/',
            $expression instanceof Node\Expr\AssignOp\Mod => '%',
            $expression instanceof Node\Expr\AssignOp\Concat => '+',
            $expression instanceof Node\Expr\AssignOp\BitwiseOr => '|',
            $expression instanceof Node\Expr\AssignOp\BitwiseAnd => '&',
            $expression instanceof Node\Expr\AssignOp\BitwiseXor => '^',
            $expression instanceof Node\Expr\AssignOp\ShiftLeft => '<<',
            $expression instanceof Node\Expr\AssignOp\ShiftRight => '>>',
            default => '+',
        };
    }
}
