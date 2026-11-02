#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

#include <bedrock/protocol/types/AttributeLayerSettingsWeight.hpp>
namespace bedrock::protocol {


class AttributeLayerSettingsWeightString : public AttributeLayerSettingsWeight {
public:
    static constexpr int64_t ID = 1;

    std::string value;

    AttributeLayerSettingsWeightString() = default;
    virtual ~AttributeLayerSettingsWeightString() = default;
    explicit AttributeLayerSettingsWeightString(const std::string& valueValue) : value(valueValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readString().value_or("");
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(value);
    }
};

} // namespace bedrock::protocol
