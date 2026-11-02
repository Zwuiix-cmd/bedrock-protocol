#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/AttributeValueColorValue.hpp>

class AttributeValueColorArray : public AttributeValueColorValue {
public:
    static constexpr int64_t ID = 1;

    uint32_t value;

    AttributeValueColorArray() = default;
    virtual ~AttributeValueColorArray() = default;
    explicit AttributeValueColorArray(const uint32_t& valueValue) : value(valueValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readUnsignedInt().value_or(0);
    }

    void write(BinaryStream& stream) const override {
        stream.writeUnsignedInt(value);
    }
};
