#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <optional>

namespace bedrock::protocol {


class DynamicValue {
public:

    DynamicValue() = default;
    virtual ~DynamicValue() = default;

    virtual int32_t getTypeId() const = 0;
    virtual void read(ReadOnlyBinaryStream& stream) = 0;
    virtual void write(BinaryStream& stream) const = 0;

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
};

} // namespace bedrock::protocol
