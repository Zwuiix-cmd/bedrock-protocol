#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/biome/chunkgen/BiomeElementData.hpp>

class BiomeSurfaceMaterialAdjustmentData {
public:
    std::vector<std::shared_ptr<BiomeElementData>> adjustments;

    BiomeSurfaceMaterialAdjustmentData() = default;
    explicit BiomeSurfaceMaterialAdjustmentData(const std::vector<std::shared_ptr<BiomeElementData>>& adjustmentsValue) : adjustments(adjustmentsValue) {}

    static std::shared_ptr<BiomeSurfaceMaterialAdjustmentData> create(ReadOnlyBinaryStream& stream) {
		auto adjustments = std::vector<std::shared_ptr<BiomeElementData>>();
		auto adjustmentsCount = stream.readUnsignedVarInt().value_or(0);
		const bool adjustmentsCountAvailable = stream.canReadCollection(adjustmentsCount);
		for (std::size_t i = 0; i < adjustmentsCount && adjustmentsCountAvailable && !stream.hasOverflowed(); ++i) {
		    adjustments.push_back(stream.readObject<BiomeElementData>());
		}
		return std::make_shared<BiomeSurfaceMaterialAdjustmentData>(adjustments);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(adjustments.size());
		for (const auto& adjustment : adjustments) {
			adjustment->write(stream);
		}
    }
};
