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
#include <bedrock/protocol/types/biome/chunkgen/BiomeScatterParamData.hpp>

class BiomeConsolidatedFeatureData {
public:
    std::shared_ptr<BiomeScatterParamData> scatter;
    int16_t feature;
    int16_t identifier;
    int16_t pass;
    bool useInternal;

    BiomeConsolidatedFeatureData() = default;
    explicit BiomeConsolidatedFeatureData(
        const std::shared_ptr<BiomeScatterParamData>& scatterValue,
        const int16_t& featureValue,
        const int16_t& identifierValue,
        const int16_t& passValue,
        const bool& useInternalValue
    ) :
        scatter(scatterValue),
        feature(featureValue),
        identifier(identifierValue),
        pass(passValue),
        useInternal(useInternalValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        feature = stream.readShort().value_or(0);
        identifier = stream.readShort().value_or(0);
        pass = stream.readShort().value_or(0);
        useInternal = stream.readBool().value_or(false);
        scatter = std::make_shared<BiomeScatterParamData>();
        scatter->read(stream);
    }

    virtual void write(BinaryStream& stream) const {
		scatter->write(stream);
		stream.writeShort(feature);
		stream.writeShort(identifier);
		stream.writeShort(pass);
		stream.writeBool(useInternal);
    }
};
