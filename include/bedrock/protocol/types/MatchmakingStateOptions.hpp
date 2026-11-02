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
