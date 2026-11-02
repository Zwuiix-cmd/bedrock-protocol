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

class DynamicValueList : public DynamicValue {
public:
    static constexpr int64_t ID = 5;

    std::vector<std::optional<std::shared_ptr<DynamicValue>>> values;

    DynamicValueList() = default;
    virtual ~DynamicValueList() = default;
    explicit DynamicValueList(const std::vector<std::optional<std::shared_ptr<DynamicValue>>>& valuesValue) : values(valuesValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        values = stream.readList([&]() {
            return stream.readOptional([&]() {
                return DynamicValue::decode(stream);
            });
        });
    }

    void write(BinaryStream& stream) const override {
        stream.writeList(values, [&](const auto& item) {
            stream.writeOptional(item, [&](const auto& optionalValue) {
                optionalValue->write(stream);
            });
        });
    }
};
