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
#include <bedrock/protocol/types/AttributeLayerSyncPayload.hpp>

class AttributesRemoveEnvironment : public AttributeLayerSyncPayload {
public:
    static constexpr int64_t ID = 3;

    std::string name;
    int32_t dimension;
    std::vector<std::string> attributes;

    AttributesRemoveEnvironment() = default;
    virtual ~AttributesRemoveEnvironment() = default;
    explicit AttributesRemoveEnvironment(
        const std::string& nameValue,
        const int32_t& dimensionValue,
        const std::vector<std::string>& attributesValue
    ) :
        name(nameValue),
        dimension(dimensionValue),
        attributes(attributesValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        name = stream.readString().value_or("");
        dimension = stream.readVarInt().value_or(0);
        attributes = stream.readList([&]() {
            return stream.readString().value_or("");
        });
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(name);
		stream.writeVarInt(dimension);
		stream.writeUnsignedVarInt(attributes.size());
		for (const auto& attribute : attributes) {
			stream.writeString(attribute);
		}
    }
};
