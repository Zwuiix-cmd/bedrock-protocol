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
#include <bedrock/protocol/types/furnace/FurnaceLayout.hpp>
#include <bedrock/protocol/types/furnace/FurnaceLeftTab.hpp>
#include <bedrock/protocol/types/furnace/FurnaceLayout.hpp>
#include <bedrock/protocol/types/furnace/FurnaceLeftTab.hpp>
#include <bedrock/protocol/types/furnace/FurnaceLayout.hpp>
#include <bedrock/protocol/types/furnace/FurnaceLeftTab.hpp>

class FurnaceOptions {
public:
    FurnaceLeftTab leftFurnaceTab;
    bool filtering;
    FurnaceLayout layout;

    FurnaceOptions() = default;
    explicit FurnaceOptions(
        const FurnaceLeftTab& leftFurnaceTabValue,
        const bool& filteringValue,
        const FurnaceLayout& layoutValue
    ) :
        leftFurnaceTab(leftFurnaceTabValue),
        filtering(filteringValue),
        layout(layoutValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        filtering = stream.readBool().value_or(false);
        leftFurnaceTab = static_cast<FurnaceLeftTab>(stream.readVarInt().value_or(0));
        layout = static_cast<FurnaceLayout>(stream.readVarInt().value_or(0));
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVarInt(static_cast<std::underlying_type_t<std::remove_cvref_t<decltype(leftFurnaceTab)>>>(leftFurnaceTab));
		stream.writeBool(filtering);
		stream.writeVarInt(static_cast<std::underlying_type_t<std::remove_cvref_t<decltype(layout)>>>(layout));
    }
};
