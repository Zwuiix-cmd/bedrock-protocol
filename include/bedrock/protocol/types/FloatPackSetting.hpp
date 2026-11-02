#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>

#include <bedrock/protocol/types/PackSetting.hpp>
namespace bedrock::protocol {


class FloatPackSetting : public PackSetting {
public:
    static constexpr auto ID = PackSettingType::Float;

    float value;

    FloatPackSetting() = default;
    virtual ~FloatPackSetting() = default;
    explicit FloatPackSetting(const std::string& nameValue, const float& valueValue) : PackSetting(nameValue), value(valueValue) {}

    static std::shared_ptr<FloatPackSetting> create(ReadOnlyBinaryStream& stream, const std::string& name) {
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

} // namespace bedrock::protocol
