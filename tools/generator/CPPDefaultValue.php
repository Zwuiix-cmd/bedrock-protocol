<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator;

class CPPDefaultValue
{
    public static function member(string $type, bool $enum = false): ?string
    {
        $type = trim($type);
        if ($type === '' || str_starts_with($type, 'std::optional<')) {
            return null;
        }
        if ($enum) {
            return 'static_cast<' . $type . '>(0)';
        }

        return match ($type) {
            'bool' => 'false',
            'float' => '0.0f',
            'double' => '0.0',
            'std::string', 'std::string_view' => '""',
            'glm::vec2' => 'glm::vec2(0.0f)',
            'glm::vec3' => 'glm::vec3(0.0f)',
            'glm::ivec3' => 'glm::ivec3(0)',
            'std::monostate' => 'std::monostate{}',
            default => self::isInteger($type) ? '0' : null,
        };
    }

    private static function isInteger(string $type): bool
    {
        return in_array($type, [
            'char',
            'signed char',
            'unsigned char',
            'short',
            'short int',
            'unsigned short',
            'unsigned short int',
            'int',
            'unsigned int',
            'long',
            'unsigned long',
            'long long',
            'unsigned long long',
            'int8_t',
            'uint8_t',
            'int16_t',
            'uint16_t',
            'int32_t',
            'uint32_t',
            'int64_t',
            'uint64_t',
            'std::size_t',
        ], true);
    }
}
