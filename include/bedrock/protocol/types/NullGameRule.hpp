#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

#include <bedrock/protocol/types/GameRule.hpp>
namespace bedrock::protocol {


class NullGameRule : public GameRule {
public:
    static constexpr int64_t ID = 0;


    NullGameRule() = default;
    virtual ~NullGameRule() = default;
    explicit NullGameRule(const bool& isPlayerModifiableValue) : GameRule(isPlayerModifiableValue) {}

    static std::shared_ptr<NullGameRule> create(ReadOnlyBinaryStream& stream, const bool& isPlayerModifiable) {
		return std::make_shared<NullGameRule>(isPlayerModifiable);
    }

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        (void)stream;
    }

    void write(BinaryStream& stream) const override {
        (void)stream;
    }
};

} // namespace bedrock::protocol
