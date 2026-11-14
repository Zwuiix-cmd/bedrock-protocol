#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

namespace bedrock::protocol {


class Attribute {
public:
    std::string id;
    float min;
    float max;
    float current;

    Attribute() = default;
    explicit Attribute(
        const std::string& idValue,
        const float& minValue,
        const float& maxValue,
        const float& currentValue
    ) :
        id(idValue),
        min(minValue),
        max(maxValue),
        current(currentValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        id = stream.readString().value_or("");
        min = stream.readFloat().value_or(0.0f);
        max = stream.readFloat().value_or(0.0f);
        current = stream.readFloat().value_or(0.0f);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(id);
        stream.writeFloat(min);
        stream.writeFloat(max);
        stream.writeFloat(current);
    }
};

} // namespace bedrock::protocol
