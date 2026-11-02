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
#include <bedrock/protocol/types/biome/chunkgen/BiomeElementData.hpp>

class BiomeSurfaceMaterialAdjustmentData {
public:
    std::vector<std::shared_ptr<BiomeElementData>> adjustments;

    BiomeSurfaceMaterialAdjustmentData() = default;
    explicit BiomeSurfaceMaterialAdjustmentData(const std::vector<std::shared_ptr<BiomeElementData>>& adjustmentsValue) : adjustments(adjustmentsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        adjustments = stream.readObjectList<BiomeElementData>();
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(adjustments.size());
		for (const auto& adjustment : adjustments) {
			adjustment->write(stream);
		}
    }
};
