<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
return ItemStackRequestAction::create(stream);
BEFORE,
<<<'AFTER'
const auto type = stream.readUnsignedVarInt().value_or(0);
            const auto innerType = stream.readUnsignedByte().value_or(0);
            if (innerType != getInnerActionType(type)) {
                stream.markOverflowed();
                return std::shared_ptr<ItemStackRequestAction>{};
            }
            return ItemStackRequestAction::create(stream, type);
AFTER],
    [<<<'BEFORE'
ItemStackRequestActionType::INNER_TYPES[action->getTypeId()]
BEFORE,
<<<'AFTER'
getInnerActionType(action->getTypeId())
AFTER],
    [<<<'BEFORE'
    ItemStackRequest() = default;
BEFORE,
<<<'AFTER'
    static uint8_t getInnerActionType(uint32_t type) {
        if (type > 18) {
            throw std::out_of_range("Invalid item stack request action");
        }
        // Inner discriminators 7 and 8 are reserved for retired actions.
        return static_cast<uint8_t>(type < 7 ? type : type + 2);
    }

    ItemStackRequest() = default;
AFTER],
];
