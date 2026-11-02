#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>


class CommandParameterRawData {
public:
    std::string name;
    uint32_t typeInfo;
    bool optional;
    uint8_t flags;

    CommandParameterRawData() = default;
    explicit CommandParameterRawData(
        const std::string& nameValue,
        const uint32_t& typeInfoValue,
        const bool& optionalValue,
        const uint8_t& flagsValue
    ) :
        name(nameValue),
        typeInfo(typeInfoValue),
        optional(optionalValue),
        flags(flagsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        typeInfo = stream.readUnsignedInt().value_or(0);
        optional = stream.readBool().value_or(false);
        flags = stream.readUnsignedByte().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		stream.writeUnsignedInt(typeInfo);
		stream.writeBool(optional);
		stream.writeUnsignedByte(flags);
    }
};
