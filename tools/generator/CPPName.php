<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator;

final class CPPName
{
    public static function type(string $phpType): string
    {
        $phpType = ltrim($phpType, '?\\');
        $phpType = preg_replace('/<.*$/', '', $phpType) ?? $phpType;
        $phpType = preg_replace('/\[\]$/', '', $phpType) ?? $phpType;
        $short = basename(str_replace('\\', '/', $phpType));
        return self::identifier($short === '' ? 'AnyValue' : $short);
    }

    public static function identifier(string $name): string
    {
        $name = preg_replace('/[^A-Za-z0-9_]/', '_', $name) ?? 'Generated';
        if ($name === '' || ctype_digit($name[0])) {
            $name = '_' . $name;
        }
        $reserved = ['alignas', 'alignof', 'and', 'and_eq', 'asm', 'auto', 'bitand', 'bitor', 'bool', 'break', 'case', 'catch', 'char', 'char8_t', 'char16_t', 'char32_t', 'class', 'compl', 'concept', 'const', 'consteval', 'constexpr', 'constinit', 'const_cast', 'continue', 'co_await', 'co_return', 'co_yield', 'decltype', 'default', 'delete', 'do', 'double', 'dynamic_cast', 'else', 'enum', 'explicit', 'export', 'extern', 'false', 'float', 'for', 'friend', 'goto', 'if', 'inline', 'int', 'long', 'mutable', 'namespace', 'new', 'noexcept', 'not', 'not_eq', 'nullptr', 'operator', 'or', 'or_eq', 'private', 'protected', 'public', 'reflexpr', 'register', 'reinterpret_cast', 'requires', 'return', 'short', 'signed', 'sizeof', 'static', 'static_assert', 'static_cast', 'struct', 'switch', 'synchronized', 'template', 'this', 'thread_local', 'throw', 'true', 'try', 'typedef', 'typeid', 'typename', 'union', 'unsigned', 'using', 'virtual', 'void', 'volatile', 'wchar_t', 'while', 'xor', 'xor_eq'];
        return in_array($name, $reserved, true) || $name === 'NULL' ? $name . '_' : $name;
    }

    public static function enumCase(string $name): string
    {
        $parts = preg_split('/[^A-Za-z0-9]+/', strtolower($name), -1, PREG_SPLIT_NO_EMPTY) ?: [];
        $result = '';
        foreach ($parts as $part) {
            $result .= ucfirst($part);
        }
        return self::identifier($result === '' ? 'Value' : $result);
    }

    public static function packetId(string $packetName): string
    {
        $name = str_ends_with($packetName, 'Packet')
            ? substr($packetName, 0, -strlen('Packet'))
            : $packetName;
        return self::identifier($name);
    }

    public static function include(string $path): string
    {
        return '#include <bedrock/protocol/' . str_replace('\\', '/', $path) . '>';
    }
}
