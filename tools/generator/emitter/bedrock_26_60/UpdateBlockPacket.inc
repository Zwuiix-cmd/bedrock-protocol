<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
flags = magic_enum::enum_cast<Flag>( stream.readUnsignedVarInt().value_or(0) ).value_or(Flag::None);
BEFORE,
<<<'AFTER'
flags = static_cast<Flag>(stream.readUnsignedVarInt().value_or(0));
AFTER],
];
