#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <utility>
#include <map>

#include <bedrock/protocol/types/cereal/DynamicValue.hpp>
namespace bedrock::protocol {


class DynamicValueMap : public DynamicValue {
public:
    static constexpr int64_t ID = 6;

    std::map<std::string, std::optional<std::shared_ptr<DynamicValue>>> value;

    DynamicValueMap() = default;
    virtual ~DynamicValueMap() = default;
    explicit DynamicValueMap(const std::map<std::string, std::optional<std::shared_ptr<DynamicValue>>>& valueValue) : value(valueValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        const auto valueCount = stream.readUnsignedInt().value_or(0);
        const bool valueCountAvailable = stream.canReadCollection(valueCount);
        for (std::size_t index = 0; index < valueCount && valueCountAvailable && !stream.hasOverflowed(); ++index) {
            auto valueKey = stream.readString().value_or("");
            auto valueValue = stream.readOptional([&]() {
	return DynamicValue::create(stream);
});
            value.emplace(std::move(valueKey), std::move(valueValue));
        }
    }

    void write(BinaryStream& stream) const override {
        stream.writeUnsignedInt(value.size());
        for (const auto& [key, item] : value) {
            stream.writeString(key);
            stream.writeOptional(item, [&](const auto& optionalValue) {
            optionalValue->write(stream);
        });
        }
    }
};

} // namespace bedrock::protocol
