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

class BoolPackSetting : public PackSetting {
public:
    static constexpr auto ID = PackSettingType::Bool;

    bool value;

    BoolPackSetting() = default;
    virtual ~BoolPackSetting() = default;
    explicit BoolPackSetting(const std::string& nameValue, const bool& valueValue) : PackSetting(nameValue), value(valueValue) {
		(void)0;
		value = valueValue;
    }

    static std::shared_ptr<BoolPackSetting> read(ReadOnlyBinaryStream& stream, const std::string& name) {
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
