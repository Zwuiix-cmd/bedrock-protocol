#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

namespace bedrock::protocol {


class BiomeCoordinateData {
public:
    int32_t minValueType;
    int16_t minValue;
    int32_t maxValueType;
    int16_t maxValue;
    uint32_t gridOffset;
    uint32_t gridStepSize;
    int32_t distribution;

    BiomeCoordinateData() = default;
    explicit BiomeCoordinateData(
        const int32_t& minValueTypeValue,
        const int16_t& minValueValue,
        const int32_t& maxValueTypeValue,
        const int16_t& maxValueValue,
        const uint32_t& gridOffsetValue,
        const uint32_t& gridStepSizeValue,
        const int32_t& distributionValue
    ) :
        minValueType(minValueTypeValue),
        minValue(minValueValue),
        maxValueType(maxValueTypeValue),
        maxValue(maxValueValue),
        gridOffset(gridOffsetValue),
        gridStepSize(gridStepSizeValue),
        distribution(distributionValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        minValueType = stream.readVarInt().value_or(0);
        minValue = stream.readShort().value_or(0);
        maxValueType = stream.readVarInt().value_or(0);
        maxValue = stream.readShort().value_or(0);
        gridOffset = stream.readUnsignedInt().value_or(0);
        gridStepSize = stream.readUnsignedInt().value_or(0);
        distribution = stream.readVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVarInt(minValueType);
		stream.writeShort(minValue);
		stream.writeVarInt(maxValueType);
		stream.writeShort(maxValue);
		stream.writeUnsignedInt(gridOffset);
		stream.writeUnsignedInt(gridStepSize);
		stream.writeVarInt(distribution);
    }
};

} // namespace bedrock::protocol
