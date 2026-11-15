#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/cereal/DynamicValue.hpp>
namespace bedrock::protocol {


class DynamicValueLong : public DynamicValue {
public:
    static constexpr int64_t ID = 2;

    int64_t value;

    DynamicValueLong() = default;
    virtual ~DynamicValueLong() = default;
    explicit DynamicValueLong(const int64_t& valueValue) : value(valueValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readLong().value_or(0);
    }

    void write(BinaryStream& stream) const override {
        stream.writeLong(value);
    }
};

} // namespace bedrock::protocol
