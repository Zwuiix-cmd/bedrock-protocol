#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <bedrock/protocol/types/AttributeEnvironment.hpp>
#include <bedrock/protocol/types/AttributeLayerSyncPayload.hpp>

class AttributesUpdateEnvironment : public AttributeLayerSyncPayload {
public:
    static constexpr int64_t ID = 2;

    std::string name;
    uint32_t dimension;
    std::vector<std::shared_ptr<AttributeEnvironment>> attributes;

    AttributesUpdateEnvironment() = default;
    virtual ~AttributesUpdateEnvironment() = default;
    explicit AttributesUpdateEnvironment(
        const std::string& nameValue,
        const uint32_t& dimensionValue,
        const std::vector<std::shared_ptr<AttributeEnvironment>>& attributesValue
    ) :
        name(nameValue),
        dimension(dimensionValue),
        attributes(attributesValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        name = stream.readString().value_or("");
        dimension = stream.readUnsignedVarInt().value_or(0);
        attributes = stream.readObjectList<AttributeEnvironment>();
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(name);
		stream.writeUnsignedVarInt(dimension);
		stream.writeUnsignedVarInt(attributes.size());
		for (const auto& attribute : attributes) {
			attribute->write(stream);
		}
    }
};
