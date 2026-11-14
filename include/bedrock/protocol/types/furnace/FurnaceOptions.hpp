#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>

#include <bedrock/protocol/types/furnace/FurnaceLayout.hpp>
#include <bedrock/protocol/types/furnace/FurnaceLeftTab.hpp>
namespace bedrock::protocol {


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
        leftFurnaceTab = magic_enum::enum_cast<FurnaceLeftTab>(
            stream.readVarInt().value_or(0)
        ).value_or(FurnaceLeftTab::None);
        filtering = stream.readBool().value_or(false);
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

} // namespace bedrock::protocol
