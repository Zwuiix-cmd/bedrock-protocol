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
#include <bedrock/protocol/types/skin/PersonaPieceTintColor.hpp>
#include <bedrock/protocol/types/skin/PersonaSkinPiece.hpp>
#include <bedrock/protocol/types/skin/SkinAnimation.hpp>
#include <bedrock/protocol/types/skin/SkinData.hpp>
#include <bedrock/protocol/types/skin/SkinImage.hpp>

class ClientDataToSkinDataHelper {
public:

    ClientDataToSkinDataHelper() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        (void)stream;
    }

    virtual void write(BinaryStream& stream) const {
        (void)stream;
    }
};
