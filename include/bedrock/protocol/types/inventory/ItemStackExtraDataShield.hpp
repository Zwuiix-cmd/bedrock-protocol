#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <bedrock/protocol/types/inventory/ItemStackExtraData.hpp>

class ItemStackExtraDataShield : public ItemStackExtraData {
public:
    int64_t blockingTick;

    ItemStackExtraDataShield() = default;
    virtual ~ItemStackExtraDataShield() = default;
    explicit ItemStackExtraDataShield(
        const std::optional<std::shared_ptr<CompoundTag>>& nbtValue,
        const std::vector<std::string>& canPlaceOnValue,
        const std::vector<std::string>& canDestroyValue,
        const int64_t& blockingTickValue
    ) :
        ItemStackExtraData(nbtValue, canPlaceOnValue, canDestroyValue),
        blockingTick(blockingTickValue) {
		(void)0;
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        blockingTick = stream.readLong().value_or(0);
    }

    void write(BinaryStream& stream) const override {
        stream.writeLong(blockingTick);
    }
};
