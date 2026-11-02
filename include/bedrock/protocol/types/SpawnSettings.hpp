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
    enum class Biome : uint8_t {
        TypeDefault = 0,
        TypeUserDefined = 1,
    };

    Biome biomeType;
    std::string biomeName;
    int32_t dimension;

    SpawnSettings() = default;
    explicit SpawnSettings(const Biome& biomeTypeValue, const std::string& biomeNameValue, const int32_t& dimensionValue) : biomeType(biomeTypeValue), biomeName(biomeNameValue), dimension(dimensionValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        biomeType = magic_enum::enum_cast<Biome>(
            stream.readUnsignedShort().value_or(0)
        ).value_or(Biome::TypeDefault);
        biomeName = stream.readString().value_or("");
        dimension = stream.readVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedShort(magic_enum::enum_integer(biomeType));
		stream.writeString(biomeName);
		stream.writeVarInt(dimension);
    }
};
