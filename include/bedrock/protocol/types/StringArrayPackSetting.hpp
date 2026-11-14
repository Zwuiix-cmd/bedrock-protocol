#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <bedrock/protocol/types/PackSetting.hpp>
namespace bedrock::protocol {


class StringArrayPackSetting : public PackSetting {
public:
    static constexpr int64_t ID = 3;

    std::vector<std::string> value;

    StringArrayPackSetting() = default;
    virtual ~StringArrayPackSetting() = default;
    explicit StringArrayPackSetting(const std::string& nameValue, const std::vector<std::string>& valueValue) : PackSetting(nameValue), value(valueValue) {}

    static std::shared_ptr<StringArrayPackSetting> create(ReadOnlyBinaryStream& stream, const std::string& name) {
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

} // namespace bedrock::protocol
