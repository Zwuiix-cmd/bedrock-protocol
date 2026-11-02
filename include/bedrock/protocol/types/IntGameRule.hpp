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

class IntGameRule : public GameRule {
public:
    static constexpr int64_t ID = 2;

    uint32_t value;

    IntGameRule() = default;
    virtual ~IntGameRule() = default;
    explicit IntGameRule(const uint32_t& valueValue, const bool& isPlayerModifiableValue) : GameRule(isPlayerModifiableValue), value(valueValue) {
		(void)0;
		value = valueValue;
    }

    static std::shared_ptr<IntGameRule> decode(ReadOnlyBinaryStream& stream, const bool& isPlayerModifiable, const bool& isStartGame) {
		return std::make_shared<IntGameRule>((isStartGame ? stream.readUnsignedVarInt().value_or(0) : stream.readUnsignedInt().value_or(0)), isPlayerModifiable);
    }

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readUnsignedVarInt().value_or(0);
    }

    void write(BinaryStream& stream, const bool& isStartGame) const override {
		if (isStartGame) {
			stream.writeUnsignedVarInt(value);
		} else {
			stream.writeUnsignedInt(value);
		}
    }
};
