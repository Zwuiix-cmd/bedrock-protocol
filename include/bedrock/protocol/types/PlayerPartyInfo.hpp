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
