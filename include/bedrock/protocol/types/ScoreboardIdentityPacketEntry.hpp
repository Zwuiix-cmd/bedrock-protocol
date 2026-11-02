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

class ScoreboardIdentityPacketEntry {
public:
    int64_t scoreboardId;
    std::optional<int32_t> actorUniqueId;

    ScoreboardIdentityPacketEntry() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        scoreboardId = stream.readVarLong().value_or(0);
        actorUniqueId = stream.readOptional([&]() {
			return stream.readInt().value_or(0);
		});
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeVarLong(scoreboardId);
        stream.writeOptional(actorUniqueId, [&](const auto& optionalValue) {
			stream.writeInt(optionalValue);
		});
    }
};
