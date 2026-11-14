#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

namespace bedrock::protocol {


class SystemDiagnosticTimingInfo {
public:
    std::string displayName;
    uint64_t systemIndex;
    uint64_t timeInNS;
    uint8_t percentOfTotal;

    SystemDiagnosticTimingInfo() = default;
    explicit SystemDiagnosticTimingInfo(
        const std::string& displayNameValue,
        const uint64_t& systemIndexValue,
        const uint64_t& timeInNSValue,
        const uint8_t& percentOfTotalValue
    ) :
        displayName(displayNameValue),
        systemIndex(systemIndexValue),
        timeInNS(timeInNSValue),
        percentOfTotal(percentOfTotalValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        displayName = stream.readString().value_or("");
        systemIndex = stream.readUnsignedLong().value_or(0);
        timeInNS = stream.readUnsignedLong().value_or(0);
        percentOfTotal = stream.readUnsignedByte().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(displayName);
		stream.writeUnsignedLong(systemIndex);
		stream.writeUnsignedLong(timeInNS);
		stream.writeUnsignedByte(percentOfTotal);
    }
};

} // namespace bedrock::protocol
