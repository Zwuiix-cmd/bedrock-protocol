#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <bedrock/protocol/types/biome/chunkgen/BiomeDefinitionChunkGenData.hpp>
namespace bedrock::protocol {


class BiomeDefinitionEntry {
public:
    std::string biomeName;
    int32_t id;
    float temperature;
    float downfall;
    float foliageSnow;
    float depth;
    float scale;
    uint32_t mapWaterColor;
    bool rain;
    std::optional<std::vector<std::string>> tags;
    std::optional<std::shared_ptr<BiomeDefinitionChunkGenData>> chunkGenData;

    BiomeDefinitionEntry() = default;
    explicit BiomeDefinitionEntry(
        const std::string& biomeNameValue,
        const int32_t& idValue,
        const float& temperatureValue,
        const float& downfallValue,
        const float& foliageSnowValue,
        const float& depthValue,
        const float& scaleValue,
        const uint32_t& mapWaterColorValue,
        const bool& rainValue,
        const std::optional<std::vector<std::string>>& tagsValue,
        const std::optional<std::shared_ptr<BiomeDefinitionChunkGenData>>& chunkGenDataValue
    ) :
        biomeName(biomeNameValue),
        id(idValue),
        temperature(temperatureValue),
        downfall(downfallValue),
        foliageSnow(foliageSnowValue),
        depth(depthValue),
        scale(scaleValue),
        mapWaterColor(mapWaterColorValue),
        rain(rainValue),
        tags(tagsValue),
        chunkGenData(chunkGenDataValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        biomeName = stream.readString().value_or("");
        id = stream.readInt().value_or(0);
        temperature = stream.readFloat().value_or(0.0f);
        downfall = stream.readFloat().value_or(0.0f);
        foliageSnow = stream.readFloat().value_or(0.0f);
        depth = stream.readFloat().value_or(0.0f);
        scale = stream.readFloat().value_or(0.0f);
        mapWaterColor = stream.readUnsignedInt().value_or(0);
        rain = stream.readBool().value_or(false);
        tags = stream.readOptional([&]() {
            return stream.readList([&]() {
                return stream.readString().value_or("");
            });
        });
        chunkGenData = stream.readOptional([&]() {
            return stream.readObject<BiomeDefinitionChunkGenData>();
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(biomeName);
        stream.writeInt(id);
        stream.writeFloat(temperature);
        stream.writeFloat(downfall);
        stream.writeFloat(foliageSnow);
        stream.writeFloat(depth);
        stream.writeFloat(scale);
        stream.writeUnsignedInt(mapWaterColor);
        stream.writeBool(rain);
        stream.writeOptional(tags, [&](const auto& optionalValue) {
            stream.writeList(optionalValue, [&](const auto& item) {
                stream.writeString(item);
            });
        });
        stream.writeOptional(chunkGenData, [&](const auto& optionalValue) {
            optionalValue->write(stream);
        });
    }
};

} // namespace bedrock::protocol
