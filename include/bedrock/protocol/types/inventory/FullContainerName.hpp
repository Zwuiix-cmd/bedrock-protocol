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

class FullContainerName {
public:
    uint8_t containerId;
    std::optional<int32_t> dynamicId;

    FullContainerName() = default;
    explicit FullContainerName(const uint8_t& containerIdValue, const std::optional<int32_t>& dynamicIdValue) : containerId(containerIdValue), dynamicId(dynamicIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        containerId = stream.readUnsignedByte().value_or(0);
        dynamicId = stream.readOptional([&]() {
			return stream.readInt().value_or(0);
		});
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(containerId);
		stream.writeOptional(dynamicId, [&](const auto& value) {
					stream.writeUnsignedInt(value);
				});
    }
};
