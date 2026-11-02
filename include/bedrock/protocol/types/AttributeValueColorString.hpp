#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

#include <bedrock/protocol/types/AttributeValueColorValue.hpp>

class AttributeValueColorString : public AttributeValueColorValue {
public:
    static constexpr int64_t ID = 0;

    std::string value;

    AttributeValueColorString() = default;
    virtual ~AttributeValueColorString() = default;
    explicit AttributeValueColorString(const std::string& valueValue) : value(valueValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readString().value_or("");
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(value);
    }
};
