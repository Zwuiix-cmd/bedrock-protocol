#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>
#include <vector>

#include <bedrock/protocol/types/AttributeLayerSyncPayload.hpp>

class AttributesRemoveEnvironment : public AttributeLayerSyncPayload {
public:
    static constexpr int64_t ID = 3;

    std::string name;
    uint32_t dimension;
    std::vector<std::string> attributes;

    AttributesRemoveEnvironment() = default;
    virtual ~AttributesRemoveEnvironment() = default;
    explicit AttributesRemoveEnvironment(
        const std::string& nameValue,
        const uint32_t& dimensionValue,
        const std::vector<std::string>& attributesValue
    ) :
        name(nameValue),
        dimension(dimensionValue),
        attributes(attributesValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        name = stream.readString().value_or("");
        dimension = stream.readUnsignedVarInt().value_or(0);
        attributes = stream.readList([&]() {
            return stream.readString().value_or("");
        });
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(name);
		stream.writeUnsignedVarInt(dimension);
		stream.writeUnsignedVarInt(attributes.size());
		for (const auto& attribute : attributes) {
			stream.writeString(attribute);
		}
    }
};
