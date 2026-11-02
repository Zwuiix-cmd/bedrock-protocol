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
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>

class DropStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 3;

    uint8_t count;
    std::shared_ptr<ItemStackRequestSlotInfo> source;
    bool randomly;

    DropStackRequestAction() = default;
    virtual ~DropStackRequestAction() = default;
    explicit DropStackRequestAction(
        const uint8_t& countValue,
        const std::shared_ptr<ItemStackRequestSlotInfo>& sourceValue,
        const bool& randomlyValue
    ) :
        count(countValue),
        source(sourceValue),
        randomly(randomlyValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        count = stream.readUnsignedByte().value_or(0);
        randomly = stream.readBool().value_or(false);
        source = std::make_shared<ItemStackRequestSlotInfo>();
        source->read(stream);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedByte(count);
		source->write(stream);
		stream.writeBool(randomly);
    }
};
