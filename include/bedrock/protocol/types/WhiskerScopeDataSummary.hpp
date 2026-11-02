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

class WhiskerScopeDataSummary {
public:
    std::string label;
    std::string indentation;
    uint64_t totalHighCostNS;
    uint64_t totalMidCostNS;
    uint64_t totalLowCostNS;

    WhiskerScopeDataSummary() = default;
    explicit WhiskerScopeDataSummary(
        const std::string& labelValue,
        const std::string& indentationValue,
        const uint64_t& totalHighCostNSValue,
        const uint64_t& totalMidCostNSValue,
        const uint64_t& totalLowCostNSValue
    ) :
        label(labelValue),
        indentation(indentationValue),
        totalHighCostNS(totalHighCostNSValue),
        totalMidCostNS(totalMidCostNSValue),
        totalLowCostNS(totalLowCostNSValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        label = stream.readString().value_or("");
        indentation = stream.readString().value_or("");
        totalHighCostNS = stream.readUnsignedLong().value_or(0);
        totalMidCostNS = stream.readUnsignedLong().value_or(0);
        totalLowCostNS = stream.readUnsignedLong().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(label);
		stream.writeString(indentation);
		stream.writeUnsignedLong(totalHighCostNS);
		stream.writeUnsignedLong(totalMidCostNS);
		stream.writeUnsignedLong(totalLowCostNS);
    }
};
