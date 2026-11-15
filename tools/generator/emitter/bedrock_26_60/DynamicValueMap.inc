<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
const auto valueCount = stream.readUnsignedInt().value_or(0);
BEFORE,
<<<'AFTER'
value.clear();
        const auto valueCount = stream.readUnsignedVarInt().value_or(0);
AFTER],
    [<<<'BEFORE'
auto valueValue = stream.readOptional([&]() { return DynamicValue::create(stream); });
BEFORE,
<<<'AFTER'
auto valueValue = DynamicValue::readTyped(stream);
AFTER],
    [<<<'BEFORE'
value.emplace(std::move(valueKey), std::move(valueValue));
BEFORE,
<<<'AFTER'
if (!value.emplace(std::move(valueKey), std::move(valueValue)).second) {
                stream.markOverflowed();
                return;
            }
AFTER],
    [<<<'BEFORE'
stream.writeUnsignedInt(value.size());
BEFORE,
<<<'AFTER'
stream.writeUnsignedVarInt(static_cast<uint32_t>(value.size()));
AFTER],
    [<<<'BEFORE'
stream.writeOptional(item, [&](const auto& optionalValue) { optionalValue->write(stream); });
BEFORE,
<<<'AFTER'
DynamicValue::writeTyped(stream, item);
AFTER],
];
