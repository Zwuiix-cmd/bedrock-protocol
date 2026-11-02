#pragma once

#include <cstdint>
#include <vector>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <bedrock/protocol/types/AbilitiesLayer.hpp>

namespace bedrock::protocol {


class AbilitiesData {
public:
    int64_t targetActorUniqueId = 0;
    uint8_t playerPermission = 0;
    uint8_t commandPermission = 0;
    std::vector<AbilitiesLayer> layers;

    void read(ReadOnlyBinaryStream& stream) {
        targetActorUniqueId = stream.readLong().value_or(0);
        playerPermission = stream.readUnsignedByte().value_or(0);
        commandPermission = stream.readUnsignedByte().value_or(0);

        const auto layerCount = stream.readUnsignedByte().value_or(0);
        const bool layerCountAvailable = stream.canReadCollection(layerCount);
        for (uint8_t i = 0; i < layerCount && layerCountAvailable && !stream.hasOverflowed(); ++i) {
            AbilitiesLayer layer;
            layer.read(stream);
            layers.push_back(layer);
        }
    }

    void write(BinaryStream& stream) const {
        stream.writeLong(targetActorUniqueId);
        stream.writeUnsignedByte(playerPermission);
        stream.writeUnsignedByte(commandPermission);
        stream.writeUnsignedByte(static_cast<uint8_t>(layers.size()));
        for (const auto& layer : layers) {
            layer.write(stream);
        }
    }
};

} // namespace bedrock::protocol
