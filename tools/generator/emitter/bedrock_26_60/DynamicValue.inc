<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
static std::shared_ptr<DynamicValue> create(ReadOnlyBinaryStream& stream);
BEFORE,
<<<'AFTER'
static std::shared_ptr<DynamicValue> create(ReadOnlyBinaryStream& stream);
    static std::shared_ptr<DynamicValue> create(ReadOnlyBinaryStream& stream, uint32_t type);

    static std::optional<std::shared_ptr<DynamicValue>> readTyped(ReadOnlyBinaryStream& stream) {
        auto value = create(stream);
        return value ? std::optional{std::move(value)} : std::nullopt;
    }

    static void writeTyped(BinaryStream& stream, const std::optional<std::shared_ptr<DynamicValue>>& value) {
        const auto* data = value ? value->get() : nullptr;
        stream.writeUnsignedInt(data ? data->getTypeId() : 0);
        if (data) {
            data->write(stream);
        }
    }
AFTER],
    [<<<'BEFORE'
#include <memory>
BEFORE,
<<<'AFTER'
#include <memory>
#include <optional>
AFTER],
];
