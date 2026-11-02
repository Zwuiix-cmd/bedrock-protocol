<?php

declare(strict_types=1);

use PhpParser\Node;
use PhpParser\Node\Expr\MethodCall;
use PhpParser\Node\Expr\StaticCall;
use PhpParser\Node\Identifier;
use PhpParser\Node\Name;
use kodiak\bedrockprotocol\generator\CPPName;
use kodiak\bedrockprotocol\generator\Configuration;
use kodiak\bedrockprotocol\generator\CPPTranslationCatalog;
use kodiak\bedrockprotocol\generator\parser\ProtocolSourceParser;
use kodiak\bedrockprotocol\generator\source\GitSourceRepository;

$root = dirname(__DIR__, 2);
$autoload = $root . DIRECTORY_SEPARATOR . 'vendor' . DIRECTORY_SEPARATOR . 'autoload.php';
if (!is_file($autoload)) {
    fwrite(STDERR, "Composer dependencies are missing. Run composer install first.\n");
    exit(1);
}

require $autoload;

$source = null;
$sourceRoot = $argv[1] ?? null;
$failures = [];
$checkedPackets = 0;
$checkedDeclarations = 0;
$checkedOperations = 0;
$skippedOperations = 0;

try {
    $configuration = Configuration::fromArguments($root, []);
    if ($sourceRoot === null || $sourceRoot === '') {
        $repository = new GitSourceRepository();
        $source = $repository->resolve($configuration);
        $sourceRoot = $source['root'];
    } elseif (!is_dir($sourceRoot)) {
        throw new RuntimeException('Source directory does not exist: ' . $sourceRoot);
    }

    $model = (new ProtocolSourceParser())->parse($sourceRoot);
    $catalog = new CPPTranslationCatalog($configuration->translation);
    $packetIds = packetIds($root . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock' . DIRECTORY_SEPARATOR . 'protocol' . DIRECTORY_SEPARATOR . 'PacketIds.hpp');

    foreach ($model['packets'] as $packet) {
        $name = (string) ($packet['name'] ?? '');
        $networkId = (int) ($packet['networkId'] ?? -1);
        $checkedPackets++;
        $packetIdName = str_ends_with($name, 'Packet') ? substr($name, 0, -strlen('Packet')) : $name;
        if (!isset($packetIds[$packetIdName])) {
            $failures[] = $name . ': missing from generated PacketIds.hpp';
        } elseif ($packetIds[$packetIdName] !== $networkId) {
            $failures[] = sprintf('%s: network ID is %d in PHP, %d in C++', $name, $networkId, $packetIds[$packetIdName]);
        }

        $header = $root . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock' . DIRECTORY_SEPARATOR . 'protocol' . DIRECTORY_SEPARATOR . $name . '.hpp';
        if (!is_file($header)) {
            $failures[] = $name . ': generated packet header is missing';
            continue;
        }
        $checkedDeclarations++;
        checkReadWriteInterface($name, $header, $failures);
        $analysis = serializerOperations($packet, $skippedOperations);
        checkOperations($name, $header, $analysis, $checkedOperations, $failures);
    }

    checkCriticalCodecContracts($root, $sourceRoot, $failures);

    foreach ($model['declarations'] as $declaration) {
        if (!is_array($declaration) || !shouldCheckDeclaration($declaration, $catalog)) {
            continue;
        }
        $name = (string) ($declaration['name'] ?? '');
        $header = generatedHeader($root, $declaration, $catalog);
        if ($header === null) {
            continue;
        }
        if (!is_file($header)) {
            $failures[] = $name . ': generated declaration header is missing';
            continue;
        }
        $checkedDeclarations++;
        if (($declaration['kind'] ?? null) !== 'enum') {
            checkReadWriteInterface($name, $header, $failures);
        }
        $analysis = serializerOperations($declaration, $skippedOperations);
        checkOperations($name, $header, $analysis, $checkedOperations, $failures);
    }

    printf(
        "parity checked %d packets, %d declarations, %d serializer operations (%d unsupported operations skipped)\n",
        $checkedPackets,
        $checkedDeclarations,
        $checkedOperations,
        $skippedOperations,
    );
    if ($failures !== []) {
        fwrite(STDERR, implode(PHP_EOL, $failures) . PHP_EOL);
        exit(1);
    }
    echo "PHP/C++ protocol parity passed\n";
} catch (Throwable $error) {
    fwrite(STDERR, 'parity failed: ' . $error->getMessage() . PHP_EOL);
    exit(1);
} finally {
    if (is_array($source)) {
        $repository->cleanup($source);
    }
}

/** @return array<string, int> */
function packetIds(string $path): array
{
    $contents = file_get_contents($path);
    if ($contents === false) {
        throw new RuntimeException('Unable to read generated PacketIds.hpp');
    }
    preg_match_all('/^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(-?\d+)\s*,/m', $contents, $matches, PREG_SET_ORDER);
    $result = [];
    foreach ($matches as $match) {
        if ($match[1] !== 'Unknown') {
            $result[$match[1]] = (int) $match[2];
        }
    }
    return $result;
}

function shouldCheckDeclaration(array $declaration, CPPTranslationCatalog $catalog): bool
{
    $name = (string) ($declaration['name'] ?? '');
    if ($catalog->isRuntimeDeclaration($name)) {
        return false;
    }
    if (($declaration['kind'] ?? null) === 'enum') {
        return true;
    }
    if (!in_array($declaration['kind'] ?? null, ['class', 'interface'], true)) {
        return false;
    }
    if (($declaration['networkId'] ?? null) !== null) {
        return false;
    }
    if (!str_starts_with((string) ($declaration['file'] ?? ''), 'src/types/')) {
        return false;
    }
    return !$catalog->isRuntimeDeclaration($name)
        && !in_array($name, ['AbilitiesData', 'AbilitiesLayer', 'PropertySyncData'], true);
}

function generatedHeader(string $root, array $declaration, CPPTranslationCatalog $catalog): ?string
{
    $name = (string) ($declaration['name'] ?? '');
    if ($name === '') {
        return null;
    }
    $generatedName = CPPName::type($name);
    $alias = $catalog->typeAlias($generatedName);
    if ($alias !== null && preg_match('/^[A-Za-z_][A-Za-z0-9_]*$/', $alias) === 1) {
        $generatedName = $alias;
    }
    if (($declaration['networkId'] ?? null) !== null) {
        $relative = $generatedName . '.hpp';
    } else {
        $file = str_replace('\\', '/', (string) ($declaration['file'] ?? ''));
        if (!str_starts_with($file, 'src/types/')) {
            return null;
        }
        $relative = substr($file, strlen('src/types/'));
        $relative = preg_replace('/\.php$/i', '.hpp', $relative) ?? $relative;
        $directory = dirname($relative);
        $relative = ($directory === '.' ? '' : $directory . '/') . $generatedName . '.hpp';
        $relative = 'types/' . $relative;
    }
    return $root . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock' . DIRECTORY_SEPARATOR . 'protocol' . DIRECTORY_SEPARATOR . str_replace('/', DIRECTORY_SEPARATOR, $relative);
}

/** @return array{read: array<string, int>, write: array<string, int>, complex: bool} */
function serializerOperations(array $declaration, int &$skipped): array
{
    $operations = ['read' => [], 'write' => [], 'complex' => false];
    foreach ($declaration['methods'] ?? [] as $method) {
        $methodName = strtolower((string) ($method['name'] ?? ''));
        $methodDirection = match ($methodName) {
            'read', 'decode', 'decodepayload' => 'read',
            'write', 'encode', 'encodepayload' => 'write',
            default => null,
        };
        if ($methodDirection === null) {
            continue;
        }
        if (containsControlFlow($method['body'] ?? [])) {
            $operations['complex'] = true;
        }
        walkNodes($method['body'] ?? [], function (Node $node) use (&$operations, &$skipped): void {
            $mapped = match ($node::class) {
                StaticCall::class => mappedStaticCall($node),
                MethodCall::class => mappedMethodCall($node),
                default => null,
            };
            if ($mapped === null) {
                if ($node instanceof StaticCall && isSerializerCall($node)) {
                    $skipped++;
                }
                return;
            }
            [$direction, $name] = $mapped;
            $operations[$direction][$name] = ($operations[$direction][$name] ?? 0) + 1;
        });
    }
    return $operations;
}

/** @param callable(Node): void $callback */
function walkNodes(mixed $value, callable $callback): void
{
    if ($value instanceof Node) {
        $callback($value);
        foreach ($value->getSubNodeNames() as $name) {
            walkNodes($value->{$name}, $callback);
        }
        return;
    }
    if (is_array($value)) {
        foreach ($value as $item) {
            walkNodes($item, $callback);
        }
    }
}

/** @return array{0: string, 1: string}|null */
function mappedStaticCall(StaticCall $call): ?array
{
    if (!$call->class instanceof Name || !$call->name instanceof Identifier) {
        return null;
    }
    $class = strtolower($call->class->getLast());
    $name = strtolower($call->name->toString());
    $direction = str_starts_with($name, 'get') || str_starts_with($name, 'read') ? 'read' : (str_starts_with($name, 'put') || str_starts_with($name, 'write') ? 'write' : null);
    if ($direction === null) {
        return null;
    }
    if ($class === 'varint') {
        $mapped = match (true) {
            str_contains($name, 'unsignedlong') => $direction === 'read' ? 'readUnsignedVarLong' : 'writeUnsignedVarLong',
            str_contains($name, 'signedlong') => $direction === 'read' ? 'readVarLong' : 'writeVarLong',
            str_contains($name, 'unsignedint') => $direction === 'read' ? 'readUnsignedVarInt' : 'writeUnsignedVarInt',
            str_contains($name, 'signedint') => $direction === 'read' ? 'readVarInt' : 'writeVarInt',
            default => null,
        };
        return $mapped === null ? null : [$direction, $mapped];
    }
    if ($class === 'byte') {
        return [$direction, str_contains($name, 'unsigned') ? ($direction === 'read' ? 'readUnsignedByte' : 'writeUnsignedByte') : ($direction === 'read' ? 'readByte' : 'writeByte')];
    }
    if ($class === 'le' || $class === 'be') {
        $suffix = $class === 'be' ? 'BE' : '';
        $base = match (true) {
            str_contains($name, 'bytearray') => 'ByteArray',
            str_contains($name, 'float') => 'Float',
            str_contains($name, 'unsignedlong') => 'UnsignedLong',
            str_contains($name, 'signedlong') => 'Long',
            str_contains($name, 'unsignedint') => 'UnsignedInt',
            str_contains($name, 'signedint') => 'Int',
            str_contains($name, 'unsignedshort') => 'UnsignedShort',
            str_contains($name, 'signedshort') => 'Short',
            default => null,
        };
        return $base === null ? null : [$direction, ($direction === 'read' ? 'read' : 'write') . $base . $suffix];
    }
    if ($class !== 'commontypes') {
        return null;
    }
    $mapped = [
        'getbool' => 'readBool',
        'putbool' => 'writeBool',
        'getstring' => 'readString',
        'putstring' => 'writeString',
        'getvector2' => 'readVec2',
        'putvector2' => 'writeVec2',
        'getvector3' => 'readVec3',
        'putvector3' => 'writeVec3',
        'getblockposition' => 'readSignedBlockPos',
        'putblockposition' => 'writeSignedBlockPos',
        'getuuid' => 'readUUID',
        'putuuid' => 'writeUUID',
        'getactoruniqueid' => 'readActorUniqueID',
        'putactoruniqueid' => 'writeActorUniqueID',
        'getactorruntimeid' => 'readActorRuntimeID',
        'putactorruntimeid' => 'writeActorRuntimeID',
        'getrotationbyte' => 'readRotationByte',
        'putrotationbyte' => 'writeRotationByte',
        'readrecipenetid' => 'readUnsignedVarInt',
        'writerecipenetid' => 'writeUnsignedVarInt',
        'readcreativeitemnetid' => 'readUnsignedVarInt',
        'writecreativeitemnetid' => 'writeUnsignedVarInt',
        'readitemstacknetidvariant' => 'readVarInt',
        'writeitemstacknetidvariant' => 'writeVarInt',
        'readitemstackrequestid' => 'readVarInt',
        'writeitemstackrequestid' => 'writeVarInt',
        'readlegacyitemstackrequestid' => 'readVarInt',
        'writelegacyitemstackrequestid' => 'writeVarInt',
        'readserveritemstackid' => 'readVarInt',
        'writeserveritemstackid' => 'writeVarInt',
    ];
    return isset($mapped[$name]) ? [$direction, $mapped[$name]] : null;
}

/** @return array{0: string, 1: string}|null */
function mappedMethodCall(MethodCall $call): ?array
{
    if (!$call->name instanceof Identifier || !$call->var instanceof Node\Expr\Variable) {
        return null;
    }
    $variable = strtolower((string) $call->var->name);
    if (!in_array($variable, ['in', 'out', 'stream'], true)) {
        return null;
    }
    $name = $call->name->toString();
    if (in_array(strtolower($name), ['readbytearray', 'writebytearray'], true)) {
        return null;
    }
    $direction = str_starts_with(strtolower($name), 'read') ? 'read' : (str_starts_with(strtolower($name), 'write') ? 'write' : null);
    return $direction === null ? null : [$direction, $name];
}

function checkCriticalCodecContracts(string $root, string $sourceRoot, array &$failures): void
{
    $source = file_get_contents($sourceRoot . DIRECTORY_SEPARATOR . 'src' . DIRECTORY_SEPARATOR . 'InventoryTransactionPacket.php');
    $header = file_get_contents($root . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock' . DIRECTORY_SEPARATOR . 'protocol' . DIRECTORY_SEPARATOR . 'InventoryTransactionPacket.hpp');
    if ($source !== false && $header !== false
        && str_contains($source, 'readLegacyItemStackRequestId')
        && (!str_contains($header, 'readVarInt()') || !str_contains($header, 'writeVarInt('))) {
        $failures[] = 'InventoryTransactionPacket: legacy request ID is not translated as signed VarInt';
    }

    $playerAuthInput = file_get_contents($root . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock' . DIRECTORY_SEPARATOR . 'protocol' . DIRECTORY_SEPARATOR . 'PlayerAuthInputPacket.hpp');
    if ($playerAuthInput !== false
        && str_contains($playerAuthInput, 'blockActions->push_back')
        && !str_contains($playerAuthInput, 'blockActions.emplace();')) {
        $failures[] = 'PlayerAuthInputPacket: block action optional is not engaged before push_back';
    }

    $startGame = file_get_contents($root . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock' . DIRECTORY_SEPARATOR . 'protocol' . DIRECTORY_SEPARATOR . 'StartGamePacket.hpp');
    if ($startGame !== false) {
        assertHeaderOrder(
            'StartGamePacket',
            $startGame,
            [
                'yaw = stream.readFloat()',
                'levelSettings = std::make_shared<LevelSettings>()',
                'levelId = stream.readString()',
                'blockPalette.push_back',
                'multiplayerCorrelationId = stream.readString()',
                'playerActorProperties = NetworkNbtSerializer::read(stream)',
            ],
            $failures,
        );
        if (str_contains($startGame, 'readValue<std::monostate>')) {
            $failures[] = 'StartGamePacket: unsupported placeholder reached the generated reader';
        }
    }
}

/** @param list<string> $needles */
function assertHeaderOrder(string $name, string $contents, array $needles, array &$failures): void
{
    $offset = 0;
    foreach ($needles as $needle) {
        $position = strpos($contents, $needle, $offset);
        if ($position === false) {
            $failures[] = $name . ': missing or reordered codec fragment ' . $needle;
            return;
        }
        $offset = $position + strlen($needle);
    }
}

function isSerializerCall(StaticCall $call): bool
{
    return $call->class instanceof Name
        && in_array(strtolower($call->class->getLast()), ['commontypes', 'byte', 'le', 'be', 'varint'], true);
}

/** @param array{read: array<string, int>, write: array<string, int>, complex: bool} $analysis @param int $checkedOperations */
function checkOperations(string $name, string $header, array $analysis, int &$checkedOperations, array &$failures): void
{
    $contents = file_get_contents($header);
    if ($contents === false) {
        $failures[] = $name . ': unable to read generated header';
        return;
    }
    foreach (['read', 'write'] as $direction) {
        $expected = $analysis[$direction];
        foreach ($expected as $method => $count) {
            $checkedOperations += $count;
            if ($analysis['complex']) {
                continue;
            }
            $actual = streamMethodCount($contents, $method);
            if ($actual < $count) {
                $failures[] = sprintf('%s: %s expects %d stream.%s call(s), generated header has %d', $name, $direction, $count, $method, $actual);
            }
        }
    }
}

function checkReadWriteInterface(string $name, string $header, array &$failures): void
{
    $contents = file_get_contents($header);
    if ($contents === false) {
        return;
    }
    if (preg_match('/\benum\s+class\s+[A-Za-z_][A-Za-z0-9_]*/', $contents) === 1) {
        return;
    }
    if (preg_match('/\b(?:virtual\s+)?(?:static\s+)?(?:std::shared_ptr<[^>]+>|void)\s+read\s*\(/', $contents) !== 1) {
        $failures[] = $name . ': generated declaration has no read method';
    }
    if (preg_match('/\b(?:virtual\s+)?void\s+write\s*\(/', $contents) !== 1) {
        $failures[] = $name . ': generated declaration has no write method';
    }
}

function streamMethodCount(string $contents, string $method): int
{
    $equivalents = [
        'readActorRuntimeID' => ['readActorRuntimeID', 'readUnsignedVarLong'],
        'writeActorRuntimeID' => ['writeActorRuntimeID', 'writeUnsignedVarLong'],
    ][$method] ?? [$method];
    $count = 0;
    foreach ($equivalents as $equivalent) {
        $matches = preg_match_all('/\bstream\.' . preg_quote($equivalent, '/') . '\s*\(/', $contents);
        $count += $matches === false ? 0 : $matches;
    }
    return $count;
}

function containsControlFlow(mixed $value): bool
{
    if ($value instanceof Node) {
        if ($value instanceof Node\Stmt\If_
            || $value instanceof Node\Stmt\For_
            || $value instanceof Node\Stmt\Foreach_
            || $value instanceof Node\Stmt\While_
            || $value instanceof Node\Stmt\Do_
            || $value instanceof Node\Stmt\Switch_
            || $value instanceof Node\Stmt\TryCatch
            || $value instanceof Node\Expr\Ternary
            || $value instanceof Node\Expr\Match_
        ) {
            return true;
        }
        if ($value instanceof StaticCall
            && $value->class instanceof Name
            && strtolower($value->class->getLast()) === 'commontypes'
            && $value->name instanceof Identifier
            && in_array(strtolower($value->name->toString()), ['readoptional', 'writeoptional'], true)) {
            return true;
        }
        foreach ($value->getSubNodeNames() as $name) {
            if (containsControlFlow($value->{$name})) {
                return true;
            }
        }
        return false;
    }
    if (is_array($value)) {
        foreach ($value as $item) {
            if (containsControlFlow($item)) {
                return true;
            }
        }
    }
    return false;
}
