<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
        const auto trustedSkinFlagRaw = stream.readString().value_or("");
        trustedSkinFlag = magic_enum::enum_cast<TrustedSkinFlag>(
            std::string_view(trustedSkinFlagRaw)
        ).value_or(TrustedSkinFlag::Unset);

BEFORE,
<<<'AFTER'

AFTER],
    [<<<'BEFORE'
        override = stream.readBool().value_or(false);
BEFORE,
<<<'AFTER'
        override = stream.readBool().value_or(false);
        trustedSkinFlag = magic_enum::enum_cast<TrustedSkinFlag>(stream.readString().value_or("")).value_or(TrustedSkinFlag::Unset);
AFTER],
    [<<<'BEFORE'
        stream.writeString(magic_enum::enum_name(trustedSkinFlag));

BEFORE,
<<<'AFTER'

AFTER],
    [<<<'BEFORE'
        stream.writeBool(override);
BEFORE,
<<<'AFTER'
        stream.writeBool(override);
        stream.writeString(magic_enum::enum_name(trustedSkinFlag));
AFTER],
];
