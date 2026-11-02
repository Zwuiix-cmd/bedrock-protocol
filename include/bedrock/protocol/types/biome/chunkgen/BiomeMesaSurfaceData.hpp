#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>


class BiomeMesaSurfaceData {
public:
    uint32_t clayMaterial;
    uint32_t hardClayMaterial;
    bool brycePillars;
    bool forest;

    BiomeMesaSurfaceData() = default;
    explicit BiomeMesaSurfaceData(
        const uint32_t& clayMaterialValue,
        const uint32_t& hardClayMaterialValue,
        const bool& brycePillarsValue,
        const bool& forestValue
    ) :
        clayMaterial(clayMaterialValue),
        hardClayMaterial(hardClayMaterialValue),
        brycePillars(brycePillarsValue),
        forest(forestValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        clayMaterial = stream.readUnsignedInt().value_or(0);
        hardClayMaterial = stream.readUnsignedInt().value_or(0);
        brycePillars = stream.readBool().value_or(false);
        forest = stream.readBool().value_or(false);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedInt(clayMaterial);
		stream.writeUnsignedInt(hardClayMaterial);
		stream.writeBool(brycePillars);
		stream.writeBool(forest);
    }
};
