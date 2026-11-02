#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>

#include <bedrock/protocol/types/PackSetting.hpp>

class StringPackSetting : public PackSetting {
public:
    static constexpr int64_t ID = 2;

    std::string value;

    StringPackSetting() = default;
    virtual ~StringPackSetting() = default;
    explicit StringPackSetting(const std::string& nameValue, const std::string& valueValue) : PackSetting(nameValue), value(valueValue) {
		(void)0;
		value = valueValue;
    }

    static std::shared_ptr<StringPackSetting> create(ReadOnlyBinaryStream& stream, const std::string& name) {
		return std::make_shared<StringPackSetting>(name, stream.readString().value_or(""));
    }

    PackSettingType getTypeId() const override { return static_cast<PackSettingType>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readString().value_or("");
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(value);
    }
};
