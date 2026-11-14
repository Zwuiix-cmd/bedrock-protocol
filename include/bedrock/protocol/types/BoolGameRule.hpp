#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

#include <bedrock/protocol/types/GameRule.hpp>
namespace bedrock::protocol {


class BoolGameRule : public GameRule {
public:
    static constexpr int64_t ID = 1;

    bool value;

    BoolGameRule() = default;
    virtual ~BoolGameRule() = default;
    explicit BoolGameRule(const bool& valueValue, const bool& isPlayerModifiableValue) : GameRule(isPlayerModifiableValue), value(valueValue) {}

    static std::shared_ptr<BoolGameRule> create(ReadOnlyBinaryStream& stream, const bool& isPlayerModifiable) {
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

} // namespace bedrock::protocol
