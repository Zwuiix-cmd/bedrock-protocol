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
