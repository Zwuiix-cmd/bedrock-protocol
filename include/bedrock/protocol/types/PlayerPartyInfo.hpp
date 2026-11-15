#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

namespace bedrock::protocol {


class PlayerPartyInfo {
public:
    std::string partyId;
    bool partyLeader;

    PlayerPartyInfo() = default;
    explicit PlayerPartyInfo(const std::string& partyIdValue, const bool& partyLeaderValue) : partyId(partyIdValue), partyLeader(partyLeaderValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        partyId = stream.readString().value_or("");
        partyLeader = stream.readBool().value_or(false);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(partyId);
		stream.writeBool(partyLeader);
    }
};

} // namespace bedrock::protocol
