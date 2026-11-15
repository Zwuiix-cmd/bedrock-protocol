<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
auto expectedInnerType = (INNER_TYPES[entry->type] ? INNER_TYPES[entry->type] : "unknown");
BEFORE,
<<<'AFTER'
const uint8_t expectedInnerType = entry->type == static_cast<uint32_t>(Type::Add) ? 0 : 1;
AFTER],
    [<<<'BEFORE'
magic_enum::enum_integer(entry->type) == magic_enum::enum_integer(Type::Add)
BEFORE,
<<<'AFTER'
entry->type == static_cast<uint32_t>(Type::Add)
AFTER],
    [<<<'BEFORE'
magic_enum::enum_integer(entry->type) == magic_enum::enum_integer(Type::Remove)
BEFORE,
<<<'AFTER'
entry->type == static_cast<uint32_t>(Type::Remove)
AFTER],
    [<<<'BEFORE'
stream.writeUnsignedByte(INNER_TYPES[entry->type]);
BEFORE,
<<<'AFTER'
stream.writeUnsignedByte(entry->type == static_cast<uint32_t>(Type::Add) ? 0 : 1);
AFTER],
];
