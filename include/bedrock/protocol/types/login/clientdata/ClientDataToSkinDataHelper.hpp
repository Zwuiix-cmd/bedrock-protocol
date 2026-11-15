#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

#include <bedrock/protocol/types/skin/PersonaPieceTintColor.hpp>
#include <bedrock/protocol/types/skin/PersonaSkinPiece.hpp>
#include <bedrock/protocol/types/skin/SkinAnimation.hpp>
#include <bedrock/protocol/types/skin/SkinData.hpp>
#include <bedrock/protocol/types/skin/SkinImage.hpp>
#include <bedrock/protocol/types/skin/PersonaSkinPieceType.hpp>
#include <bedrock/protocol/types/skin/SkinArmSizeType.hpp>
namespace bedrock::protocol {


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

} // namespace bedrock::protocol
