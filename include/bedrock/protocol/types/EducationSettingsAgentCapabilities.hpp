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
