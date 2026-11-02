#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <optional>

namespace bedrock::protocol {


class EducationSettingsAgentCapabilities {
public:
    std::optional<bool> canModifyBlocks;

    EducationSettingsAgentCapabilities() = default;
    explicit EducationSettingsAgentCapabilities(const std::optional<bool>& canModifyBlocksValue) : canModifyBlocks(canModifyBlocksValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        canModifyBlocks = stream.readOptional([&]() {
            return stream.readBool().value_or(false);
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeOptional(canModifyBlocks, [&](const auto& value) {
			stream.writeBool(value);
		});
    }
};

} // namespace bedrock::protocol
