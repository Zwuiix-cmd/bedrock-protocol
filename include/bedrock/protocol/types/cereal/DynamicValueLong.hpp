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

class DynamicValueLong : public DynamicValue {
public:
    static constexpr int64_t ID = 2;

    int64_t value;

    DynamicValueLong() = default;
    virtual ~DynamicValueLong() = default;
    explicit DynamicValueLong(const int64_t& valueValue) : value(valueValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readLong().value_or(0);
    }

    void write(BinaryStream& stream) const override {
        stream.writeLong(value);
    }
};
