#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <optional>

namespace bedrock::protocol {


class ScoreboardIdentityPacketEntry {
public:
    int64_t scoreboardId;
    std::optional<int64_t> actorUniqueId;

    ScoreboardIdentityPacketEntry() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        scoreboardId = stream.readVarLong().value_or(0);
        actorUniqueId = stream.readOptional([&]() {
            return stream.readActorUniqueID().value_or(0);
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeVarLong(scoreboardId);
        stream.writeOptional(actorUniqueId, [&](const auto& optionalValue) {
            stream.writeActorUniqueID(optionalValue);
        });
    }
};

} // namespace bedrock::protocol
