#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>


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
		if (constraintsCount > ReadOnlyBinaryStream::MaxCollectionSize) {
		    throw std::runtime_error("protocol collection is too large");
		}
		for (std::size_t i = 0; i < constraintsCount; i++) {
		    constraints.push_back(stream.readUnsignedByte().value_or(0));
		}
		return std::make_shared<CommandEnumConstraintRawData>(affectedValueIndex, enumIndex, constraints);
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
