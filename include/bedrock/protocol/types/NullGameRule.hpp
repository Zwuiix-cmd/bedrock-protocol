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

class NullGameRule : public GameRule {
public:
    static constexpr int64_t ID = 0;


    NullGameRule() = default;
    virtual ~NullGameRule() = default;
    explicit NullGameRule(const bool& isPlayerModifiableValue) : GameRule(isPlayerModifiableValue) {
		(void)0;
    }

    static std::shared_ptr<NullGameRule> decode(ReadOnlyBinaryStream& stream, const bool& isPlayerModifiable) {
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
