#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/biome/chunkgen/BiomeCoordinateData.hpp>
namespace bedrock::protocol {


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

    static std::shared_ptr<BiomeScatterParamData> create(ReadOnlyBinaryStream& stream) {
		auto coordinates = std::vector<std::shared_ptr<BiomeCoordinateData>>();
		auto coordinatesCount = stream.readUnsignedVarInt().value_or(0);
		const bool coordinatesCountAvailable = stream.canReadCollection(coordinatesCount);
		for (std::size_t i = 0; i < coordinatesCount && coordinatesCountAvailable && !stream.hasOverflowed(); ++i) {
		    coordinates.push_back(stream.readObject<BiomeCoordinateData>());
		}
		auto evalOrder = stream.readVarInt().value_or(0);
		auto chancePercentType = stream.readVarInt().value_or(0);
		auto chancePercent = stream.readShort().value_or(0);
		auto chanceNumerator = stream.readInt().value_or(0);
		auto chanceDenominator = stream.readInt().value_or(0);
		auto iterationsType = stream.readVarInt().value_or(0);
		auto iterations = stream.readShort().value_or(0);
		return std::make_shared<BiomeScatterParamData>(coordinates, evalOrder, chancePercentType, static_cast<int32_t>(chancePercent), chanceNumerator, chanceDenominator, iterationsType, static_cast<int32_t>(iterations));
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
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

} // namespace bedrock::protocol
