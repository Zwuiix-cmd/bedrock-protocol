#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

namespace bedrock::protocol {


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

} // namespace bedrock::protocol
