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

class CraftingRecipeBlockName {
public:
    static constexpr std::string_view CARTOGRAPHY_TABLE = "cartography_table";
    static constexpr std::string_view CRAFTING_TABLE = "crafting_table";
    static constexpr std::string_view STONECUTTER = "stonecutter";
    static constexpr std::string_view SMITHING_TABLE = "smithing_table";

    CraftingRecipeBlockName() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        (void)stream;
    }

    virtual void write(BinaryStream& stream) const {
        (void)stream;
    }
};
