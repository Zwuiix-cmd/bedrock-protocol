#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/ddui/update/DataStoreUpdateValue.hpp>
namespace bedrock::protocol {


class DoubleDataStoreUpdateValue : public DataStoreUpdateValue {
public:
    static constexpr int64_t ID = 0;

    double value;

    DoubleDataStoreUpdateValue() = default;
    virtual ~DoubleDataStoreUpdateValue() = default;
    explicit DoubleDataStoreUpdateValue(const double& valueValue) : value(valueValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readDouble().value_or(0.0);
    }

    void write(BinaryStream& stream) const override {
        stream.writeDouble(value);
    }
};

} // namespace bedrock::protocol
