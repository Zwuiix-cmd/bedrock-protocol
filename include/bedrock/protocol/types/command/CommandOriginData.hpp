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

class CommandOriginData {
public:
    static constexpr std::string_view ORIGIN_PLAYER = "player";
    static constexpr std::string_view ORIGIN_BLOCK = "commandblock";
    static constexpr std::string_view ORIGIN_MINECART_BLOCK = "minecartcommandblock";
    static constexpr std::string_view ORIGIN_DEV_CONSOLE = "devconsole";
    static constexpr std::string_view ORIGIN_TEST = "test";
    static constexpr std::string_view ORIGIN_AUTOMATION_PLAYER = "automationplayer";
    static constexpr std::string_view ORIGIN_CLIENT_AUTOMATION = "clientautomation";
    static constexpr std::string_view ORIGIN_DEDICATED_SERVER = "dedicatedserver";
    static constexpr std::string_view ORIGIN_ENTITY = "entity";
    static constexpr std::string_view ORIGIN_VIRTUAL = "virtual";
    static constexpr std::string_view ORIGIN_GAME_ARGUMENT = "gameargument";
    static constexpr std::string_view ORIGIN_ENTITY_SERVER = "entityserver";
    static constexpr std::string_view ORIGIN_PRECOMPILED = "precompiled";
    static constexpr std::string_view ORIGIN_GAME_DIRECTOR_ENTITY_SERVER = "gamedirectorentityserver";
    static constexpr std::string_view ORIGIN_SCRIPTING = "scripting";
    static constexpr std::string_view ORIGIN_EXECUTE_CONTEXT = "executecontext";
    std::string type;
    std::string uuid;
    std::string requestId;
    int64_t playerActorUniqueId;

    CommandOriginData() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        type = stream.readString().value_or("");
        requestId = stream.readString().value_or("");
        playerActorUniqueId = stream.readLong().value_or(0);
        uuid = stream.readUUID().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(type);
        stream.writeString(requestId);
        stream.writeLong(playerActorUniqueId);
        stream.writeUUID(uuid);
    }
};
