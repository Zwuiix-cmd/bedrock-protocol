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
#include <map>
#include <bedrock/protocol/types/cereal/DynamicValue.hpp>
#include <bedrock/protocol/types/cereal/DynamicValue.hpp>

class DynamicValueMap : public DynamicValue {
public:
    static constexpr int64_t ID = 6;

    std::map<std::string, std::optional<std::shared_ptr<DynamicValue>>> value;

    DynamicValueMap() = default;
    virtual ~DynamicValueMap() = default;
    explicit DynamicValueMap(const std::map<std::string, std::optional<std::shared_ptr<DynamicValue>>>& valueValue) : value(valueValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = ([&]() {
    std::optional<std::map<std::string, std::optional<std::shared_ptr<DynamicValue>>>> result;
    const auto count = stream.readUnsignedInt().value_or(0);
    for (uint32_t i = 0; i < count; ++i) {
        auto key = stream.readString().value_or("");
        auto value = stream.readOptional([&]() {
	return DynamicValue::decode(stream);
});
        result.emplace(std::move(key), std::move(value));
    }
    return result;
}());
    }

    void write(BinaryStream& stream) const override {
        stream.writeUnsignedInt(value.size());
for (const auto& [key, item] : value) {
    stream.writeString(key);
    stream.writeOptional(item, [&](const auto& optionalValue) {
	if (optionalValue) optionalValue->write(stream);
});
}
    }
};
