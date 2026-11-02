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
#include <bedrock/protocol/types/GameRule.hpp>

class BoolGameRule : public GameRule {
public:
    static constexpr int64_t ID = 1;

    bool value;

    BoolGameRule() = default;
    virtual ~BoolGameRule() = default;
    explicit BoolGameRule(const bool& valueValue, const bool& isPlayerModifiableValue) : GameRule(isPlayerModifiableValue), value(valueValue) {
		(void)0;
		value = valueValue;
    }

    static std::shared_ptr<BoolGameRule> decode(ReadOnlyBinaryStream& stream, const bool& isPlayerModifiable) {
		return std::make_shared<BoolGameRule>(stream.readBool().value_or(false), isPlayerModifiable);
    }

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readBool().value_or(false);
    }

    void write(BinaryStream& stream) const override {
		stream.writeBool(value);
    }
};
