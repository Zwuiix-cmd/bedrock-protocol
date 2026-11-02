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
#include <bedrock/protocol/types/entity/AttributeModifier.hpp>
#include <bedrock/protocol/types/entity/AttributeModifier.hpp>
#include <bedrock/protocol/types/entity/AttributeModifier.hpp>

class UpdateAttribute {
public:
    std::string id;
    float min;
    float max;
    float current;
    float defaultMin;
    float defaultMax;
    float default_;
    std::vector<std::shared_ptr<AttributeModifier>> modifiers;

    UpdateAttribute() = default;
    explicit UpdateAttribute(
        const std::string& idValue,
        const float& minValue,
        const float& maxValue,
        const float& currentValue,
        const float& defaultMinValue,
        const float& defaultMaxValue,
        const float& default_Value,
        const std::vector<std::shared_ptr<AttributeModifier>>& modifiersValue
    ) :
        id(idValue),
        min(minValue),
        max(maxValue),
        current(currentValue),
        defaultMin(defaultMinValue),
        defaultMax(defaultMaxValue),
        default_(default_Value),
        modifiers(modifiersValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        id = stream.readString().value_or("");
        min = stream.readFloat().value_or(0.0f);
        max = stream.readFloat().value_or(0.0f);
        current = stream.readFloat().value_or(0.0f);
        defaultMin = stream.readFloat().value_or(0.0f);
        defaultMax = stream.readFloat().value_or(0.0f);
        default_ = stream.readFloat().value_or(0.0f);
        modifiers = stream.readList([&]() {
			return ([&]() {
			auto value = std::make_shared<AttributeModifier>();
			value->read(stream);
			return value;
			}());
		});
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeFloat(min);
		stream.writeFloat(max);
		stream.writeFloat(current);
		stream.writeFloat(defaultMin);
		stream.writeFloat(defaultMax);
		stream.writeFloat(default_);
		stream.writeString(id);
		stream.writeUnsignedVarInt(modifiers.size());
		for (const auto& modifier : modifiers) {
			modifier->write(stream);
		}
    }
};
