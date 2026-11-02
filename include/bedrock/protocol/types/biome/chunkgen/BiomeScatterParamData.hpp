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
#include <bedrock/protocol/types/biome/chunkgen/BiomeCoordinateData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeCoordinateData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeCoordinateData.hpp>

class BiomeScatterParamData {
public:
    std::vector<std::shared_ptr<BiomeCoordinateData>> coordinates;
    int32_t evalOrder;
    int32_t chancePercentType;
    int16_t chancePercent;
    int32_t chanceNumerator;
    int32_t chanceDenominator;
    int32_t iterationsType;
    int16_t iterations;

    BiomeScatterParamData() = default;
    explicit BiomeScatterParamData(
        const std::vector<std::shared_ptr<BiomeCoordinateData>>& coordinatesValue,
        const int32_t& evalOrderValue,
        const int32_t& chancePercentTypeValue,
        const int16_t& chancePercentValue,
        const int32_t& chanceNumeratorValue,
        const int32_t& chanceDenominatorValue,
        const int32_t& iterationsTypeValue,
        const int16_t& iterationsValue
    ) :
        coordinates(coordinatesValue),
        evalOrder(evalOrderValue),
        chancePercentType(chancePercentTypeValue),
        chancePercent(chancePercentValue),
        chanceNumerator(chanceNumeratorValue),
        chanceDenominator(chanceDenominatorValue),
        iterationsType(iterationsTypeValue),
        iterations(iterationsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        evalOrder = stream.readVarInt().value_or(0);
        chancePercentType = stream.readVarInt().value_or(0);
        chancePercent = stream.readShort().value_or(0);
        chanceNumerator = stream.readInt().value_or(0);
        chanceDenominator = stream.readInt().value_or(0);
        iterationsType = stream.readVarInt().value_or(0);
        iterations = stream.readShort().value_or(0);
        coordinates = stream.readList([&]() {
	return ([&]() {
	auto value = std::make_shared<BiomeCoordinateData>();
	value->read(stream);
	return value;
	}());
});
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(coordinates.size());
		for (const auto& coordinate : coordinates) {
			coordinate->write(stream);
		}
		stream.writeVarInt(evalOrder);
		stream.writeVarInt(chancePercentType);
		stream.writeShort(chancePercent);
		stream.writeInt(chanceNumerator);
		stream.writeInt(chanceDenominator);
		stream.writeVarInt(iterationsType);
		stream.writeShort(iterations);
    }
};
