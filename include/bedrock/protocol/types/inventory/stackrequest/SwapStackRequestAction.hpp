#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestSlotInfo.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>

class SwapStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 2;

    std::shared_ptr<ItemStackRequestSlotInfo> slot1;
    std::shared_ptr<ItemStackRequestSlotInfo> slot2;

    SwapStackRequestAction() = default;
    virtual ~SwapStackRequestAction() = default;
    explicit SwapStackRequestAction(
        const std::shared_ptr<ItemStackRequestSlotInfo>& slot1Value,
        const std::shared_ptr<ItemStackRequestSlotInfo>& slot2Value
    ) :
        slot1(slot1Value),
        slot2(slot2Value) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        slot1 = std::make_shared<ItemStackRequestSlotInfo>();
        slot1->read(stream);
        slot2 = std::make_shared<ItemStackRequestSlotInfo>();
        slot2->read(stream);
    }

    void write(BinaryStream& stream) const override {
		slot1->write(stream);
		slot2->write(stream);
    }
};
