#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>

#include <bedrock/protocol/types/PackSetting.hpp>
namespace bedrock::protocol {


class BoolPackSetting : public PackSetting {
public:
    static constexpr auto ID = PackSettingType::Bool;

    bool value;

    BoolPackSetting() = default;
    virtual ~BoolPackSetting() = default;
    explicit BoolPackSetting(const std::string& nameValue, const bool& valueValue) : PackSetting(nameValue), value(valueValue) {}

    static std::shared_ptr<BoolPackSetting> create(ReadOnlyBinaryStream& stream, const std::string& name) {
		return std::make_shared<BoolPackSetting>(name, stream.readBool().value_or(false));
    }

    PackSettingType getTypeId() const override { return static_cast<PackSettingType>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readBool().value_or(false);
    }

    void write(BinaryStream& stream) const override {
		stream.writeBool(value);
    }
};

} // namespace bedrock::protocol
