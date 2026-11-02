#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

namespace bedrock::protocol {


class CommandEnumConstraintRawData {
public:
    uint32_t affectedValueIndex;
    uint32_t enumIndex;
    std::vector<uint8_t> constraints;

    CommandEnumConstraintRawData() = default;
    explicit CommandEnumConstraintRawData(
        const uint32_t& affectedValueIndexValue,
        const uint32_t& enumIndexValue,
        const std::vector<uint8_t>& constraintsValue
    ) :
        affectedValueIndex(affectedValueIndexValue),
        enumIndex(enumIndexValue),
        constraints(constraintsValue) {}

    static std::shared_ptr<CommandEnumConstraintRawData> create(ReadOnlyBinaryStream& stream) {
		auto affectedValueIndex = stream.readUnsignedInt().value_or(0);
		auto enumIndex = stream.readUnsignedInt().value_or(0);
		auto constraints = std::vector<uint8_t>();
		auto constraintsCount = stream.readUnsignedVarInt().value_or(0);
		const bool constraintsCountAvailable = stream.canReadCollection(constraintsCount);
		for (std::size_t i = 0; i < constraintsCount && constraintsCountAvailable && !stream.hasOverflowed(); i++) {
		    constraints.push_back(stream.readUnsignedByte().value_or(0));
		}
		return std::make_shared<CommandEnumConstraintRawData>(static_cast<int32_t>(affectedValueIndex), static_cast<int32_t>(enumIndex), constraints);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedInt(affectedValueIndex);
		stream.writeUnsignedInt(enumIndex);
		stream.writeUnsignedVarInt(constraints.size());
		for (const auto& constraint : constraints) {
			stream.writeUnsignedByte(constraint);
		}
    }
};

} // namespace bedrock::protocol
