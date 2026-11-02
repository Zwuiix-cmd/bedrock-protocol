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
#include <bedrock/protocol/types/cereal/DynamicValue.hpp>
#include <bedrock/protocol/types/cereal/DynamicValue.hpp>
#include <bedrock/protocol/types/cereal/DynamicValue.hpp>
#include <bedrock/protocol/types/ddui/DataStoreOperation.hpp>

class DataStoreChange : public DataStoreOperation {
public:
    static constexpr int64_t ID = 1;

    std::string name;
    std::string property;
    uint32_t updateCount;
    std::optional<std::shared_ptr<DynamicValue>> data;

    DataStoreChange() = default;
    virtual ~DataStoreChange() = default;
    explicit DataStoreChange(
        const std::string& nameValue,
        const std::string& propertyValue,
        const uint32_t& updateCountValue,
        const std::optional<std::shared_ptr<DynamicValue>>& dataValue
    ) :
        name(nameValue),
        property(propertyValue),
        updateCount(updateCountValue),
        data(dataValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        name = stream.readString().value_or("");
        property = stream.readString().value_or("");
        updateCount = stream.readUnsignedInt().value_or(0);
        data = stream.readOptional([&]() {
	return DynamicValue::decode(stream);
});
    }

    void write(BinaryStream& stream) const override {
        stream.writeString(name);
        stream.writeString(property);
        stream.writeUnsignedInt(updateCount);
        stream.writeOptional(data, [&](const auto& optionalValue) {
	if (optionalValue) optionalValue->write(stream);
});
    }
};
