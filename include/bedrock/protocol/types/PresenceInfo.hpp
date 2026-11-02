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

class PresenceInfo {
public:
    std::optional<std::string> richPresenceId;

    PresenceInfo() = default;
    explicit PresenceInfo(const std::optional<std::string>& richPresenceIdValue) : richPresenceId(richPresenceIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        richPresenceId = stream.readOptional([&]() {
	return stream.readString().value_or("");
});
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeOptional(richPresenceId, [&](const auto& value) {
			stream.writeString(value);
		});
    }
};
