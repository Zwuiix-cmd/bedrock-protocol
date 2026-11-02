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

class SpawnSettings {
public:
    static constexpr int64_t BIOME_TYPE_DEFAULT = 0;
    static constexpr int64_t BIOME_TYPE_USER_DEFINED = 1;

    uint16_t biomeType;
    std::string biomeName;
    int32_t dimension;

    SpawnSettings() = default;
    explicit SpawnSettings(const uint16_t& biomeTypeValue, const std::string& biomeNameValue, const int32_t& dimensionValue) : biomeType(biomeTypeValue), biomeName(biomeNameValue), dimension(dimensionValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        biomeType = stream.readUnsignedShort().value_or(0);
        biomeName = stream.readString().value_or("");
        dimension = stream.readVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedShort(biomeType);
		stream.writeString(biomeName);
		stream.writeVarInt(dimension);
    }
};
