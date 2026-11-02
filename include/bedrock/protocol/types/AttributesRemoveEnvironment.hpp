#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
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

    static std::shared_ptr<AttributesRemoveEnvironment> create(ReadOnlyBinaryStream& stream) {
		auto name = stream.readString().value_or("");
		auto dimension = stream.readUnsignedVarInt().value_or(0);
		auto attributes = std::vector<std::string>();
		auto attributesCount = stream.readUnsignedVarInt().value_or(0);
		if (attributesCount > ReadOnlyBinaryStream::MaxCollectionSize) {
		    throw std::runtime_error("protocol collection is too large");
		}
		for (std::size_t i = 0; i < attributesCount; ++i) {
		    attributes.push_back(stream.readString().value_or(""));
		}
		return std::make_shared<AttributesRemoveEnvironment>(name, dimension, attributes);
    }

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
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
