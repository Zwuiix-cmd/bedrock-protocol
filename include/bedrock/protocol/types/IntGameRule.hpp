#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

#include <bedrock/protocol/types/GameRule.hpp>
namespace bedrock::protocol {


class IntGameRule : public GameRule {
public:
    static constexpr int64_t ID = 2;

    uint32_t value;

    IntGameRule() = default;
    virtual ~IntGameRule() = default;
    explicit IntGameRule(const uint32_t& valueValue, const bool& isPlayerModifiableValue) : GameRule(isPlayerModifiableValue), value(valueValue) {}

    static std::shared_ptr<IntGameRule> create(ReadOnlyBinaryStream& stream, const bool& isPlayerModifiable) {
		return std::make_shared<IntGameRule>(static_cast<int32_t>(stream.readUnsignedInt().value_or(0)), isPlayerModifiable);
    }

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readUnsignedInt().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedInt(value);
    }
};

} // namespace bedrock::protocol
