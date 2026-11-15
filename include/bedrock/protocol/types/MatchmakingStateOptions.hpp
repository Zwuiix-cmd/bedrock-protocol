#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <optional>
#include <string>

namespace bedrock::protocol {


class MatchmakingStateOptions {
public:
    std::optional<std::string> triggeringPlayerName;
    std::optional<bool> triggeredByLocalPlayer;

    MatchmakingStateOptions() = default;
    explicit MatchmakingStateOptions(
        const std::optional<std::string>& triggeringPlayerNameValue,
        const std::optional<bool>& triggeredByLocalPlayerValue
    ) :
        triggeringPlayerName(triggeringPlayerNameValue),
        triggeredByLocalPlayer(triggeredByLocalPlayerValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        triggeringPlayerName = stream.readOptional([&]() {
            return stream.readString().value_or("");
        });
        triggeredByLocalPlayer = stream.readOptional([&]() {
            return stream.readBool().value_or(false);
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeOptional(triggeringPlayerName, [&](const auto& value) {
			stream.writeString(value);
		});
		stream.writeOptional(triggeredByLocalPlayer, [&](const auto& value) {
			stream.writeBool(value);
		});
    }
};

} // namespace bedrock::protocol
