#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>
namespace bedrock::protocol {


class CreativeCreateStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 14;

    uint32_t creativeItemId;
    uint8_t repetitions;

    CreativeCreateStackRequestAction() = default;
    virtual ~CreativeCreateStackRequestAction() = default;
    explicit CreativeCreateStackRequestAction(const uint32_t& creativeItemIdValue, const uint8_t& repetitionsValue) : creativeItemId(creativeItemIdValue), repetitions(repetitionsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        creativeItemId = stream.readUnsignedVarInt().value_or(0);
        repetitions = stream.readUnsignedByte().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(creativeItemId);
		stream.writeUnsignedByte(repetitions);
    }
};

} // namespace bedrock::protocol
