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

class FurnaceRecipeBlockName {
public:
    static constexpr std::string_view BLAST_FURNACE = "blast_furnace";
    static constexpr std::string_view CAMPFIRE = "campfire";
    static constexpr std::string_view FURNACE = "furnace";
    static constexpr std::string_view SMOKER = "smoker";
    static constexpr std::string_view SOUL_CAMPFIRE = "soul_campfire";

    FurnaceRecipeBlockName() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        (void)stream;
    }

    virtual void write(BinaryStream& stream) const {
        (void)stream;
    }
};
