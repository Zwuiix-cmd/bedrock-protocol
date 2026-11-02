#pragma once

#include <magic_enum/magic_enum.hpp>
#include <string_view>
#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/PacketIds.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

class Packet {
public:
    static constexpr PacketIds ID = PacketIds::Unknown;

public:
    virtual ~Packet() = default;
    virtual std::string_view getName() = 0;
    virtual PacketIds getId() = 0;

    virtual void write(BinaryStream& stream) = 0;
    virtual void read(ReadOnlyBinaryStream& stream) = 0;
};