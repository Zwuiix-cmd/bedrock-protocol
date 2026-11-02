#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <vector>


class CommandEnumConstraintRawData {
public:
    uint32_t affectedValueIndex;
    uint32_t enumIndex;
    std::vector<int32_t> constraints;

    CommandEnumConstraintRawData() = default;
    explicit CommandEnumConstraintRawData(
        const uint32_t& affectedValueIndexValue,
        const uint32_t& enumIndexValue,
        const std::vector<int32_t>& constraintsValue
    ) :
        affectedValueIndex(affectedValueIndexValue),
        enumIndex(enumIndexValue),
        constraints(constraintsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        affectedValueIndex = stream.readUnsignedInt().value_or(0);
        enumIndex = stream.readUnsignedInt().value_or(0);
        constraints = stream.readList([&]() {
            return stream.readInt().value_or(0);
        });
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
