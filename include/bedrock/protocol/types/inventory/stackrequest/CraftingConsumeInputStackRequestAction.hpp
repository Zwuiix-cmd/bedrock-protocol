#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestSlotInfo.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>

class CraftingConsumeInputStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 5;

    uint8_t count;
    std::shared_ptr<ItemStackRequestSlotInfo> source;

    CraftingConsumeInputStackRequestAction() = default;
    virtual ~CraftingConsumeInputStackRequestAction() = default;
    explicit CraftingConsumeInputStackRequestAction(const uint8_t& countValue, const std::shared_ptr<ItemStackRequestSlotInfo>& sourceValue) : count(countValue), source(sourceValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        count = stream.readUnsignedByte().value_or(0);
        source = std::make_shared<ItemStackRequestSlotInfo>();
        source->read(stream);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedByte(count);
		source->write(stream);
    }
};
