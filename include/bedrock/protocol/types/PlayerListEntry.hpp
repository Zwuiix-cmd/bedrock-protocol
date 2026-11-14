#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include <bedrock/protocol/types/BuildPlatform.hpp>
#include <bedrock/protocol/types/skin/SkinData.hpp>
namespace bedrock::protocol {


class PlayerListEntry {
public:
    uint32_t type;
    std::string uuid;
    int64_t actorUniqueId;
    std::string username;
    std::shared_ptr<SkinData> skinData;
    std::string xboxUserId;
    std::string playFabId;
    std::string platformChatId;
    BuildPlatform buildPlatform = BuildPlatform::Unknown;
    bool isTeacher = false;
    bool isHost = false;
    bool isSubClient = false;
    std::optional<uint32_t> color;

    PlayerListEntry() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        type = stream.readUnsignedVarInt().value_or(0);
        actorUniqueId = stream.readActorUniqueID().value_or(0);
        username = stream.readString().value_or("");
        xboxUserId = stream.readString().value_or("");
        playFabId = stream.readString().value_or("");
        platformChatId = stream.readString().value_or("");
        buildPlatform = magic_enum::enum_cast<BuildPlatform>(
            stream.readInt().value_or(0)
        ).value_or(BuildPlatform::Unknown);
        isTeacher = stream.readBool().value_or(false);
        isHost = stream.readBool().value_or(false);
        isSubClient = stream.readBool().value_or(false);
        uuid = stream.readUUID().value_or("");
        skinData = std::make_shared<SkinData>();
        skinData->read(stream);
        color = stream.readOptional([&]() {
            return stream.readUnsignedInt().value_or(0);
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedVarInt(type);
        stream.writeActorUniqueID(actorUniqueId);
        stream.writeString(username);
        stream.writeString(xboxUserId);
        stream.writeString(playFabId);
        stream.writeString(platformChatId);
        stream.writeInt(magic_enum::enum_integer(buildPlatform));
        stream.writeBool(isTeacher);
        stream.writeBool(isHost);
        stream.writeBool(isSubClient);
        stream.writeUUID(uuid);
        skinData->write(stream);
        stream.writeOptional(color, [&](const auto& optionalValue) {
            stream.writeUnsignedInt(optionalValue);
        });
    }
};

} // namespace bedrock::protocol
