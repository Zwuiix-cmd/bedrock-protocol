#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <optional>
#include <string>

#include <bedrock/protocol/types/ScorePacketEntryAction.hpp>
namespace bedrock::protocol {


class ScorePacketEntry {
public:
    int64_t scoreboardId;
    std::optional<std::string> objectiveName;
    int32_t score;
    ScorePacketEntryAction action;
    std::optional<int64_t> actorUniqueId;
    std::optional<std::string> customName;

    ScorePacketEntry() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        action = magic_enum::enum_cast<ScorePacketEntryAction>(
            stream.readUnsignedVarInt().value_or(0)
        ).value_or(ScorePacketEntryAction::Remove);
        scoreboardId = stream.readVarLong().value_or(0);
        objectiveName = stream.readOptional([&]() {
            return stream.readString().value_or("");
        });
        score = stream.readInt().value_or(0);
        actorUniqueId = stream.readOptional([&]() {
            return stream.readActorUniqueID().value_or(0);
        });
        customName = stream.readOptional([&]() {
            return stream.readString().value_or("");
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedVarInt(magic_enum::enum_integer(action));
        stream.writeVarLong(scoreboardId);
        stream.writeOptional(objectiveName, [&](const auto& optionalValue) {
            stream.writeString(optionalValue);
        });
        stream.writeInt(score);
        stream.writeOptional(actorUniqueId, [&](const auto& optionalValue) {
            stream.writeActorUniqueID(optionalValue);
        });
        stream.writeOptional(customName, [&](const auto& optionalValue) {
            stream.writeString(optionalValue);
        });
    }
};

} // namespace bedrock::protocol
