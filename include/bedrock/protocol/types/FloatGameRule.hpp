#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

#include <bedrock/protocol/types/GameRule.hpp>

class FloatGameRule : public GameRule {
public:
    static constexpr int64_t ID = 3;

    float value;

    FloatGameRule() = default;
    virtual ~FloatGameRule() = default;
    explicit FloatGameRule(const float& valueValue, const bool& isPlayerModifiableValue) : GameRule(isPlayerModifiableValue), value(valueValue) {}

    static std::shared_ptr<FloatGameRule> create(ReadOnlyBinaryStream& stream, const bool& isPlayerModifiable) {
		return std::make_shared<FloatGameRule>(stream.readFloat().value_or(0.0f), isPlayerModifiable);
    }

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readFloat().value_or(0.0f);
    }

    void write(BinaryStream& stream, const bool& isStartGame) const override {
		stream.writeFloat(value);
    }
};
