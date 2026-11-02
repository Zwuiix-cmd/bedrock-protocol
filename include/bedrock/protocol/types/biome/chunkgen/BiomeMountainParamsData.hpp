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

class BiomeMountainParamsData {
public:
    uint32_t steepBlock;
    bool northSlopes;
    bool southSlopes;
    bool westSlopes;
    bool eastSlopes;
    bool topSlideEnabled;

    BiomeMountainParamsData() = default;
    explicit BiomeMountainParamsData(
        const uint32_t& steepBlockValue,
        const bool& northSlopesValue,
        const bool& southSlopesValue,
        const bool& westSlopesValue,
        const bool& eastSlopesValue,
        const bool& topSlideEnabledValue
    ) :
        steepBlock(steepBlockValue),
        northSlopes(northSlopesValue),
        southSlopes(southSlopesValue),
        westSlopes(westSlopesValue),
        eastSlopes(eastSlopesValue),
        topSlideEnabled(topSlideEnabledValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        steepBlock = stream.readUnsignedInt().value_or(0);
        northSlopes = stream.readBool().value_or(false);
        southSlopes = stream.readBool().value_or(false);
        westSlopes = stream.readBool().value_or(false);
        eastSlopes = stream.readBool().value_or(false);
        topSlideEnabled = stream.readBool().value_or(false);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedInt(steepBlock);
		stream.writeBool(northSlopes);
		stream.writeBool(southSlopes);
		stream.writeBool(westSlopes);
		stream.writeBool(eastSlopes);
		stream.writeBool(topSlideEnabled);
    }
};
