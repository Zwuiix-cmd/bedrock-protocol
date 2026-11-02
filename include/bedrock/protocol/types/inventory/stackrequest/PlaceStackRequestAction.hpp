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
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestSlotInfo.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestSlotInfo.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestSlotInfo.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>

class PlaceStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 1;

    uint8_t count;
    std::shared_ptr<ItemStackRequestSlotInfo> source;
    std::shared_ptr<ItemStackRequestSlotInfo> destination;

    PlaceStackRequestAction() = default;
    virtual ~PlaceStackRequestAction() = default;
    explicit PlaceStackRequestAction(
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
        source = ([&]() {
    auto value = std::make_shared<ItemStackRequestSlotInfo>();
    value->read(stream);
    return value;
}());
        destination = ([&]() {
    auto value = std::make_shared<ItemStackRequestSlotInfo>();
    value->read(stream);
    return value;
}());
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedByte(count);
		source->write(stream);
		destination->write(stream);
    }
};
