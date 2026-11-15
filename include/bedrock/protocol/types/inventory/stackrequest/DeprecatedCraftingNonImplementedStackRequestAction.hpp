#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>
namespace bedrock::protocol {


class DeprecatedCraftingNonImplementedStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 17;


    DeprecatedCraftingNonImplementedStackRequestAction() = default;
    virtual ~DeprecatedCraftingNonImplementedStackRequestAction() = default;

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        (void)stream;
    }

    void write(BinaryStream& stream) const override {
        (void)stream;
    }
};

} // namespace bedrock::protocol
