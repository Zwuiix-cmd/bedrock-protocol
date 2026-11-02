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
