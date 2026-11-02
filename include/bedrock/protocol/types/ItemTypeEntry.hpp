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
#include <bedrock/protocol/types/NetworkNbt.hpp>
#include <bedrock/protocol/types/entity/MetadataProperty.hpp>

class ItemTypeEntry {
public:
    std::string stringId;
    int16_t numericId;
    bool componentBased;
    int32_t version;
    std::shared_ptr<CompoundTag> componentNbt;

    ItemTypeEntry() = default;
    explicit ItemTypeEntry(
        const std::string& stringIdValue,
        const int16_t& numericIdValue,
        const bool& componentBasedValue,
        const int32_t& versionValue,
        const std::shared_ptr<CompoundTag>& componentNbtValue
    ) :
        stringId(stringIdValue),
        numericId(numericIdValue),
        componentBased(componentBasedValue),
        version(versionValue),
        componentNbt(componentNbtValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        stringId = stream.readString().value_or("");
        numericId = stream.readShort().value_or(0);
        componentBased = stream.readBool().value_or(false);
        version = stream.readVarInt().value_or(0);
        componentNbt = NetworkNbtSerializer::read(stream);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(stringId);
        stream.writeShort(numericId);
        stream.writeBool(componentBased);
        stream.writeVarInt(version);
        NetworkNbtSerializer::write(stream, componentNbt);
    }
};
