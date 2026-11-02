#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
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

    static std::shared_ptr<AttributesUpdateEnvironment> create(ReadOnlyBinaryStream& stream) {
		auto name = stream.readString().value_or("");
		auto dimension = stream.readUnsignedVarInt().value_or(0);
		auto attributes = std::vector<std::shared_ptr<AttributeEnvironment>>();
		auto attributesCount = stream.readUnsignedVarInt().value_or(0);
		if (attributesCount > ReadOnlyBinaryStream::MaxCollectionSize) {
		    throw std::runtime_error("protocol collection is too large");
		}
		for (std::size_t i = 0; i < attributesCount; ++i) {
		    attributes.push_back(stream.readObject<AttributeEnvironment>());
		}
		return std::make_shared<AttributesUpdateEnvironment>(name, dimension, attributes);
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
			attribute->write(stream);
		}
    }
};
