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

class ItemStackRequest {
public:
    int32_t requestId;
    std::vector<std::shared_ptr<ItemStackRequestAction>> actions;
    std::vector<std::string> filterStrings;
    int32_t filterStringCause;

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

    virtual void read(ReadOnlyBinaryStream& stream) {
        filterStringCause = stream.readInt().value_or(0);
        requestId = stream.readInt().value_or(0);
        actions = stream.readList([&]() {
			return ItemStackRequestAction::decode(stream);
		});
        filterStrings = stream.readList([&]() {
			return stream.readString().value_or("");
		});
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeInt(requestId);
		stream.writeList(actions, [&](const auto& action) {
					stream.writeUnsignedVarInt(action->getTypeId());
					stream.writeUnsignedByte(ItemStackRequestActionType::INNER_TYPES[action->getTypeId()]);
					action->write(stream);
				});
		stream.writeList(filterStrings, [&](const auto& value) {
					stream.writeString(value);
				});
		stream.writeInt(filterStringCause);
    }
};
