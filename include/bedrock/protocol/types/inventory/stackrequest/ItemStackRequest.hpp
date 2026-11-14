#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/BeaconPaymentStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/CraftRecipeAutoStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/CraftRecipeOptionalStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/CraftRecipeStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/CraftReservedStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/CraftingConsumeInputStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/CraftingCreateSpecificResultStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/CreativeCreateStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/DeprecatedCraftingNonImplementedStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/DeprecatedCraftingResultsStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/DestroyStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/DropStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/GrindstoneStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/LabTableCombineStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/LoomStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/MineBlockStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/PlaceStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/SwapStackRequestAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/TakeStackRequestAction.hpp>
namespace bedrock::protocol {


class ItemStackRequest {
public:
    int32_t requestId;
    std::vector<std::shared_ptr<ItemStackRequestAction>> actions;
    std::vector<std::string> filterStrings;
    int32_t filterStringCause;

    static uint8_t getInnerActionType(uint32_t type) {
        if (type > 18) {
            throw std::out_of_range("Invalid item stack request action");
        }
        // Inner discriminators 7 and 8 are reserved for retired actions.
        return static_cast<uint8_t>(type < 7 ? type : type + 2);
    }

    ItemStackRequest() = default;
    explicit ItemStackRequest(
        const int32_t& requestIdValue,
        const std::vector<std::shared_ptr<ItemStackRequestAction>>& actionsValue,
        const std::vector<std::string>& filterStringsValue,
        const int32_t& filterStringCauseValue
    ) :
        requestId(requestIdValue),
        actions(actionsValue),
        filterStrings(filterStringsValue),
        filterStringCause(filterStringCauseValue) {}

    static std::shared_ptr<ItemStackRequest> create(ReadOnlyBinaryStream& stream) {
        auto result = std::make_shared<ItemStackRequest>();
        result->requestId = stream.readVarInt().value_or(0);
        result->actions = stream.readList([&]() {
            const auto type = stream.readUnsignedVarInt().value_or(0);
            const auto innerType = stream.readUnsignedByte().value_or(0);
            if (innerType != getInnerActionType(type)) {
                stream.markOverflowed();
                return std::shared_ptr<ItemStackRequestAction>{};
            }
            return ItemStackRequestAction::create(stream, type);
        });
        result->filterStrings = stream.readList([&]() {
            return stream.readString().value_or("");
        });
        result->filterStringCause = stream.readInt().value_or(0);
        return result;
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVarInt(requestId);
		stream.writeList(actions, [&](const auto& action) {
					stream.writeUnsignedVarInt(action->getTypeId());
					stream.writeUnsignedByte(getInnerActionType(action->getTypeId()));
					action->write(stream);
				});
		stream.writeList(filterStrings, [&](const auto& value) {
			stream.writeString(value);
		});
		stream.writeInt(filterStringCause);
    }
};

} // namespace bedrock::protocol
