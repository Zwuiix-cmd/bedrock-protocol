#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

namespace bedrock::protocol {


class PropertySyncData {
public:
    std::vector<std::pair<uint32_t, int32_t>> intProperties;
    std::vector<std::pair<uint32_t, float>> floatProperties;

    void read(ReadOnlyBinaryStream& stream) {
        intProperties.clear();
        const auto intCount = stream.readUnsignedVarInt().value_or(0);
        const bool intCountAvailable = stream.canReadCollection(intCount);
        intProperties.reserve(intCount);
        for (uint32_t i = 0; i < intCount && intCountAvailable && !stream.hasOverflowed(); ++i) {
            intProperties.emplace_back(
                stream.readUnsignedVarInt().value_or(0),
                stream.readVarInt().value_or(0)
            );
        }

        floatProperties.clear();
        const auto floatCount = stream.readUnsignedVarInt().value_or(0);
        const bool floatCountAvailable = stream.canReadCollection(floatCount);
        floatProperties.reserve(floatCount);
        for (uint32_t i = 0; i < floatCount && floatCountAvailable && !stream.hasOverflowed(); ++i) {
            floatProperties.emplace_back(
                stream.readUnsignedVarInt().value_or(0),
                stream.readFloat().value_or(0.0f)
            );
        }
    }

    void write(BinaryStream& stream) const {
        stream.writeUnsignedVarInt(static_cast<uint32_t>(intProperties.size()));
        for (const auto& [key, value] : intProperties) {
            stream.writeUnsignedVarInt(key);
            stream.writeVarInt(value);
        }

        stream.writeUnsignedVarInt(static_cast<uint32_t>(floatProperties.size()));
        for (const auto& [key, value] : floatProperties) {
            stream.writeUnsignedVarInt(key);
            stream.writeFloat(value);
        }
    }
};

} // namespace bedrock::protocol
