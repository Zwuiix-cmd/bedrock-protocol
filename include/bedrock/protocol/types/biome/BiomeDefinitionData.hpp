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
#include <bedrock/protocol/types/biome/chunkgen/BiomeDefinitionChunkGenData.hpp>

class BiomeDefinitionData {
public:
    uint16_t nameIndex;
    uint16_t id;
    float temperature;
    float downfall;
    float foliageSnow;
    float depth;
    float scale;
    uint32_t mapWaterColor;
    bool rain;
    std::optional<std::vector<int32_t>> tagIndexes;
    std::optional<std::shared_ptr<BiomeDefinitionChunkGenData>> chunkGenData;

    BiomeDefinitionData() = default;
    explicit BiomeDefinitionData(
        const uint16_t& nameIndexValue,
        const uint16_t& idValue,
        const float& temperatureValue,
        const float& downfallValue,
        const float& foliageSnowValue,
        const float& depthValue,
        const float& scaleValue,
        const uint32_t& mapWaterColorValue,
        const bool& rainValue,
        const std::optional<std::vector<int32_t>>& tagIndexesValue,
        const std::optional<std::shared_ptr<BiomeDefinitionChunkGenData>>& chunkGenDataValue
    ) :
        nameIndex(nameIndexValue),
        id(idValue),
        temperature(temperatureValue),
        downfall(downfallValue),
        foliageSnow(foliageSnowValue),
        depth(depthValue),
        scale(scaleValue),
        mapWaterColor(mapWaterColorValue),
        rain(rainValue),
        tagIndexes(tagIndexesValue),
        chunkGenData(chunkGenDataValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        nameIndex = stream.readUnsignedShort().value_or(0);
        id = stream.readUnsignedShort().value_or(0);
        temperature = stream.readFloat().value_or(0.0f);
        downfall = stream.readFloat().value_or(0.0f);
        foliageSnow = stream.readFloat().value_or(0.0f);
        depth = stream.readFloat().value_or(0.0f);
        scale = stream.readFloat().value_or(0.0f);
        rain = stream.readBool().value_or(false);
        mapWaterColor = stream.readUnsignedInt().value_or(0);
        tagIndexes = stream.readList([&]() {
            return stream.readInt().value_or(0);
        });
        chunkGenData = stream.readOptional([&]() {
            return stream.readObject<BiomeDefinitionChunkGenData>();
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedShort(nameIndex);
		stream.writeUnsignedShort(id);
		stream.writeFloat(temperature);
		stream.writeFloat(downfall);
		stream.writeFloat(foliageSnow);
		stream.writeFloat(depth);
		stream.writeFloat(scale);
		stream.writeUnsignedInt(static_cast<uint32_t>(mapWaterColor));
		stream.writeBool(rain);
		stream.writeOptional(tagIndexes, [&](const auto& tagIndexes) {
					stream.writeUnsignedVarInt(tagIndexes.size());
					for (const auto& tag : tagIndexes) {
				stream.writeUnsignedShort(tag);
			}
				});
		stream.writeOptional(chunkGenData, [&](const auto& v) {
			v->write(stream);
		});
    }
};
