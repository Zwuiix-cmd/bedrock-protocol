#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <string>
#include <vector>

#include <bedrock/protocol/types/entity/AttributeModifier.hpp>
namespace bedrock::protocol {


class Attribute {
public:
    std::string id;
    float min;
    float max;
    float current;
    float default_;
    std::vector<std::shared_ptr<AttributeModifier>> modifiers;

    Attribute() = default;
    explicit Attribute(
        const std::string& idValue,
        const float& minValue,
        const float& maxValue,
        const float& currentValue,
        const float& default_Value,
        const std::vector<std::shared_ptr<AttributeModifier>>& modifiersValue
    ) :
        id(idValue),
        min(minValue),
        max(maxValue),
        current(currentValue),
        default_(default_Value),
        modifiers(modifiersValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        id = stream.readString().value_or("");
        min = stream.readFloat().value_or(0.0f);
        max = stream.readFloat().value_or(0.0f);
        current = stream.readFloat().value_or(0.0f);
        default_ = stream.readFloat().value_or(0.0f);
        modifiers = stream.readObjectList<AttributeModifier>();
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(id);
        stream.writeFloat(min);
        stream.writeFloat(max);
        stream.writeFloat(current);
        stream.writeFloat(default_);
        stream.writeList(modifiers, [&](const auto& item) {
            item->write(stream);
        });
    }
};

} // namespace bedrock::protocol
