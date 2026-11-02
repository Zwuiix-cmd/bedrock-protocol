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

class LegacyAuthIdentityData {
public:
    std::string displayName;
    std::string identity;
    std::string sandboxId = "RETAIL";
    std::optional<std::string> titleId;
    std::string XUID;

    LegacyAuthIdentityData() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        displayName = stream.readString().value_or("");
        identity = stream.readString().value_or("");
        sandboxId = stream.readString().value_or("");
        titleId = stream.readOptional([&]() {
			return stream.readString().value_or("");
		});
        XUID = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(displayName);
        stream.writeString(identity);
        stream.writeString(sandboxId);
        stream.writeOptional(titleId, [&](const auto& optionalValue) {
			stream.writeString(optionalValue);
		});
        stream.writeString(XUID);
    }
};
