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
        leftFurnaceTab = magic_enum::enum_cast<FurnaceLeftTab>(
            stream.readVarInt().value_or(0)
        ).value_or(FurnaceLeftTab::None);
        layout = magic_enum::enum_cast<FurnaceLayout>(
            stream.readVarInt().value_or(0)
        ).value_or(FurnaceLayout::None);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVarInt(magic_enum::enum_integer(leftFurnaceTab));
		stream.writeBool(filtering);
		stream.writeVarInt(magic_enum::enum_integer(layout));
    }
};
