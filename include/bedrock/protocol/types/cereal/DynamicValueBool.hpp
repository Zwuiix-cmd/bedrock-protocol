#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/cereal/DynamicValue.hpp>
namespace bedrock::protocol {


class DynamicValueBool : public DynamicValue {
public:
    static constexpr int64_t ID = 1;

    bool value;

    DynamicValueBool() = default;
    virtual ~DynamicValueBool() = default;
    explicit DynamicValueBool(const bool& valueValue) : value(valueValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readBool().value_or(false);
    }

    void write(BinaryStream& stream) const override {
        stream.writeBool(value);
    }
};

} // namespace bedrock::protocol
