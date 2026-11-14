#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include <bedrock/protocol/types/cereal/DynamicValue.hpp>
namespace bedrock::protocol {


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
                return DynamicValue::create(stream);
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

} // namespace bedrock::protocol
