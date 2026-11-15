#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/ddui/update/DataStoreUpdateValue.hpp>
namespace bedrock::protocol {


class DoubleDataStoreUpdateValue : public DataStoreUpdateValue {
public:
    static constexpr int64_t ID = 0;

    float value;

    DoubleDataStoreUpdateValue() = default;
    virtual ~DoubleDataStoreUpdateValue() = default;
    explicit DoubleDataStoreUpdateValue(const float& valueValue) : value(valueValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readFloat().value_or(0.0f);
    }

    void write(BinaryStream& stream) const override {
        stream.writeFloat(value);
    }
};

} // namespace bedrock::protocol
