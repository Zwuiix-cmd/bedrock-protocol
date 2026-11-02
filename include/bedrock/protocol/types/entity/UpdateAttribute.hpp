#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

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

    static std::shared_ptr<UpdateAttribute> create(ReadOnlyBinaryStream& stream) {
		auto min = stream.readFloat().value_or(0.0f);
		auto max = stream.readFloat().value_or(0.0f);
		auto current = stream.readFloat().value_or(0.0f);
		auto defaultMin = stream.readFloat().value_or(0.0f);
		auto defaultMax = stream.readFloat().value_or(0.0f);
		auto default_ = stream.readFloat().value_or(0.0f);
		auto id = stream.readString().value_or("");
		auto modifiers = std::vector<std::shared_ptr<AttributeModifier>>();
		auto modifiersCount = stream.readUnsignedVarInt().value_or(0);
		const bool modifiersCountAvailable = stream.canReadCollection(modifiersCount);
		for (std::size_t j = 0; j < modifiersCount && modifiersCountAvailable && !stream.hasOverflowed(); j++) {
		    modifiers.push_back(stream.readObject<AttributeModifier>());
		}
		return std::make_shared<UpdateAttribute>(id, min, max, current, defaultMin, defaultMax, default_, modifiers);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
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
