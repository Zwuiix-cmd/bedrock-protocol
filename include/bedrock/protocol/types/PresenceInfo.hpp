#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <optional>
#include <string>

namespace bedrock::protocol {


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

} // namespace bedrock::protocol
