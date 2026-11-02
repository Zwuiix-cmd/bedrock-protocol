#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <optional>

namespace bedrock::protocol {


class FullContainerName {
public:
    uint8_t containerId;
    std::optional<uint32_t> dynamicId;

    FullContainerName() = default;
    explicit FullContainerName(const uint8_t& containerIdValue, const std::optional<uint32_t>& dynamicIdValue) : containerId(containerIdValue), dynamicId(dynamicIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        containerId = stream.readUnsignedByte().value_or(0);
        dynamicId = stream.readOptional([&]() {
            return stream.readUnsignedInt().value_or(0);
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(containerId);
		stream.writeOptional(dynamicId, [&](const auto& value) {
			stream.writeUnsignedInt(value);
		});
    }
};

} // namespace bedrock::protocol
