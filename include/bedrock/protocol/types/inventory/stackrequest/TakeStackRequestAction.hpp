#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestSlotInfo.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>

class TakeStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 0;

    uint8_t count;
    std::shared_ptr<ItemStackRequestSlotInfo> source;
    std::shared_ptr<ItemStackRequestSlotInfo> destination;

    TakeStackRequestAction() = default;
    virtual ~TakeStackRequestAction() = default;
    explicit TakeStackRequestAction(
        const uint8_t& countValue,
        const std::shared_ptr<ItemStackRequestSlotInfo>& sourceValue,
        const std::shared_ptr<ItemStackRequestSlotInfo>& destinationValue
    ) :
        count(countValue),
        source(sourceValue),
        destination(destinationValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        count = stream.readUnsignedByte().value_or(0);
        source = std::make_shared<ItemStackRequestSlotInfo>();
        source->read(stream);
        destination = std::make_shared<ItemStackRequestSlotInfo>();
        destination->read(stream);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedByte(count);
		source->write(stream);
		destination->write(stream);
    }
};
