#pragma once

#include <cstdint>
#include <limits>
#include <magic_enum/magic_enum.hpp>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <bedrock/protocol/Forward.hpp>
#include <bedrock/protocol/types/AttributeEnvironment.hpp>
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
        attributes = stream.readList([&]() {
            return ([&]() {
            auto value = std::make_shared<AttributeEnvironment>();
            value->read(stream);
            return value;
            }());
        });
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
