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
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>

class CraftingCreateSpecificResultStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 6;

    uint8_t resultIndex;

    CraftingCreateSpecificResultStackRequestAction() = default;
    virtual ~CraftingCreateSpecificResultStackRequestAction() = default;
    explicit CraftingCreateSpecificResultStackRequestAction(const uint8_t& resultIndexValue) : resultIndex(resultIndexValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        resultIndex = stream.readUnsignedByte().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedByte(resultIndex);
    }
};
