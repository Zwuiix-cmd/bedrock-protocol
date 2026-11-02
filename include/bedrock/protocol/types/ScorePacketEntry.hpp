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

class ScorePacketEntry {
public:
    enum class Type : uint8_t {
        Player = 1,
        Entity = 2,
        FakePlayer = 3,
    };

    int64_t scoreboardId;
    std::string objectiveName;
    int32_t score;
    Type type;
    std::optional<int64_t> actorUniqueId;
    std::optional<std::string> customName;

    ScorePacketEntry() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        scoreboardId = stream.readVarLong().value_or(0);
        objectiveName = stream.readString().value_or("");
        score = stream.readInt().value_or(0);
        type = static_cast<Type>(stream.readUnsignedByte().value_or(0));
        actorUniqueId = stream.readOptional([&]() {
            return stream.readActorUniqueID().value_or(0);
        });
        customName = stream.readOptional([&]() {
            return stream.readString().value_or("");
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeVarLong(scoreboardId);
        stream.writeString(objectiveName);
        stream.writeInt(score);
        stream.writeUnsignedByte(static_cast<std::underlying_type_t<Type>>(type));
        stream.writeOptional(actorUniqueId, [&](const auto& optionalValue) {
            stream.writeActorUniqueID(optionalValue);
        });
        stream.writeOptional(customName, [&](const auto& optionalValue) {
            stream.writeString(optionalValue);
        });
    }
};
