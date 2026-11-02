#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>
namespace bedrock::protocol {


class BeaconPaymentStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 10;

    int32_t primaryEffectId;
    int32_t secondaryEffectId;

    BeaconPaymentStackRequestAction() = default;
    virtual ~BeaconPaymentStackRequestAction() = default;
    explicit BeaconPaymentStackRequestAction(const int32_t& primaryEffectIdValue, const int32_t& secondaryEffectIdValue) : primaryEffectId(primaryEffectIdValue), secondaryEffectId(secondaryEffectIdValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        primaryEffectId = stream.readVarInt().value_or(0);
        secondaryEffectId = stream.readVarInt().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeVarInt(primaryEffectId);
		stream.writeVarInt(secondaryEffectId);
    }
};

} // namespace bedrock::protocol
