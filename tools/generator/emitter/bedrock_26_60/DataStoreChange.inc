<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
data = stream.readOptional([&]() { return DynamicValue::create(stream); });
BEFORE,
<<<'AFTER'
const auto type = stream.readUnsignedInt().value_or(0);
        auto value = DynamicValue::create(stream, type);
        data = value ? std::optional{std::move(value)} : std::nullopt;
AFTER],
    [<<<'BEFORE'
stream.writeOptional(data, [&](const auto& optionalValue) { optionalValue->write(stream); });
BEFORE,
<<<'AFTER'
const auto* value = data ? data->get() : nullptr;
        stream.writeUnsignedInt(value ? value->getTypeId() : 0);
        if (value) {
            value->write(stream);
        }
AFTER],
];
