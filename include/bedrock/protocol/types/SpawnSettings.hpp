#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <string>

namespace bedrock::protocol {


class SpawnSettings {
public:
    enum class Type : uint8_t {
        Default = 0,
        UserDefined = 1,
    };

    Type biomeType;
    std::string biomeName;
    int32_t dimension;

    SpawnSettings() = default;
    explicit SpawnSettings(const Type& biomeTypeValue, const std::string& biomeNameValue, const int32_t& dimensionValue) : biomeType(biomeTypeValue), biomeName(biomeNameValue), dimension(dimensionValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        biomeType = magic_enum::enum_cast<Type>(
            stream.readUnsignedShort().value_or(0)
        ).value_or(Type{});
        biomeName = stream.readString().value_or("");
        dimension = stream.readVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedShort(magic_enum::enum_integer(biomeType));
		stream.writeString(biomeName);
		stream.writeVarInt(dimension);
    }
};

} // namespace bedrock::protocol
