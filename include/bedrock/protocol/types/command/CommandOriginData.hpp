#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <string>
#include <string_view>


class CommandOriginData {
public:
    enum class Type : uint8_t {
        Player = 0,
        Block = 1,
        MinecartBlock = 2,
        DevConsole = 3,
        Test = 4,
        AutomationPlayer = 5,
        ClientAutomation = 6,
        DedicatedServer = 7,
        Entity = 8,
        Virtual = 9,
        GameArgument = 10,
        EntityServer = 11,
        Precompiled = 12,
        GameDirectorEntityServer = 13,
        Scripting = 14,
        ExecuteContext = 15,
    };

    Type type;
    std::string uuid;
    std::string requestId;
    int64_t playerActorUniqueId;

    CommandOriginData() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        const auto typeRaw = stream.readString().value_or("");
        type = magic_enum::enum_cast<Type>(
            std::string_view(typeRaw)
        ).value_or(Type::Player);
        requestId = stream.readString().value_or("");
        playerActorUniqueId = stream.readLong().value_or(0);
        uuid = stream.readUUID().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(magic_enum::enum_name(type));
        stream.writeString(requestId);
        stream.writeLong(playerActorUniqueId);
        stream.writeUUID(uuid);
    }
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<CommandOriginData::Type>(CommandOriginData::Type value) noexcept {
    switch (value) {
        case CommandOriginData::Type::Player: return "player";
        case CommandOriginData::Type::Block: return "commandblock";
        case CommandOriginData::Type::MinecartBlock: return "minecartcommandblock";
        case CommandOriginData::Type::DevConsole: return "devconsole";
        case CommandOriginData::Type::Test: return "test";
        case CommandOriginData::Type::AutomationPlayer: return "automationplayer";
        case CommandOriginData::Type::ClientAutomation: return "clientautomation";
        case CommandOriginData::Type::DedicatedServer: return "dedicatedserver";
        case CommandOriginData::Type::Entity: return "entity";
        case CommandOriginData::Type::Virtual: return "virtual";
        case CommandOriginData::Type::GameArgument: return "gameargument";
        case CommandOriginData::Type::EntityServer: return "entityserver";
        case CommandOriginData::Type::Precompiled: return "precompiled";
        case CommandOriginData::Type::GameDirectorEntityServer: return "gamedirectorentityserver";
        case CommandOriginData::Type::Scripting: return "scripting";
        case CommandOriginData::Type::ExecuteContext: return "executecontext";
        default: return invalid_tag;
    }
}

}
