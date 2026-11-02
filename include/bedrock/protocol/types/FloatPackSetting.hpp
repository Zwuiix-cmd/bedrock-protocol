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
#include <bedrock/protocol/types/PackSetting.hpp>

class FloatPackSetting : public PackSetting {
public:
    static constexpr auto ID = PackSettingType::Float;

    float value;

    FloatPackSetting() = default;
    virtual ~FloatPackSetting() = default;
    explicit FloatPackSetting(const std::string& nameValue, const float& valueValue) : PackSetting(nameValue), value(valueValue) {
		(void)0;
		value = valueValue;
    }

    static std::shared_ptr<FloatPackSetting> read(ReadOnlyBinaryStream& stream, const std::string& name) {
		return std::make_shared<FloatPackSetting>(name, stream.readFloat().value_or(0.0f));
    }

    PackSettingType getTypeId() const override { return static_cast<PackSettingType>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readFloat().value_or(0.0f);
    }

    void write(BinaryStream& stream) const override {
		stream.writeFloat(value);
    }
};
