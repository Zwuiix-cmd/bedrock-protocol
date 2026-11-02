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

class StringArrayPackSetting : public PackSetting {
public:
    static constexpr int64_t ID = 3;

    std::vector<std::string> value;

    StringArrayPackSetting() = default;
    virtual ~StringArrayPackSetting() = default;
    explicit StringArrayPackSetting(const std::string& nameValue, const std::vector<std::string>& valueValue) : PackSetting(nameValue), value(valueValue) {
		(void)0;
		value = valueValue;
    }

    static std::shared_ptr<StringArrayPackSetting> read(ReadOnlyBinaryStream& stream, const std::string& name) {
		return std::make_shared<StringArrayPackSetting>(name, stream.readList([&]() {
            return stream.readString().value_or("");
        }));
    }

    PackSettingType getTypeId() const override { return static_cast<PackSettingType>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readList([&]() {
            return stream.readString().value_or("");
        });
    }

    void write(BinaryStream& stream) const override {
		stream.writeList(value, [&](const auto& value) {
		            stream.writeString(value);
		        });
    }
};
