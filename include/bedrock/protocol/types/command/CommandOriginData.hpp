#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <string>
#include <string_view>

namespace bedrock::protocol {


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

} // namespace bedrock::protocol

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::CommandOriginData::Type>(::bedrock::protocol::CommandOriginData::Type value) noexcept {
    switch (value) {
        case ::bedrock::protocol::CommandOriginData::Type::Player: return "player";
        case ::bedrock::protocol::CommandOriginData::Type::Block: return "commandblock";
        case ::bedrock::protocol::CommandOriginData::Type::MinecartBlock: return "minecartcommandblock";
        case ::bedrock::protocol::CommandOriginData::Type::DevConsole: return "devconsole";
        case ::bedrock::protocol::CommandOriginData::Type::Test: return "test";
        case ::bedrock::protocol::CommandOriginData::Type::AutomationPlayer: return "automationplayer";
        case ::bedrock::protocol::CommandOriginData::Type::ClientAutomation: return "clientautomation";
        case ::bedrock::protocol::CommandOriginData::Type::DedicatedServer: return "dedicatedserver";
        case ::bedrock::protocol::CommandOriginData::Type::Entity: return "entity";
        case ::bedrock::protocol::CommandOriginData::Type::Virtual: return "virtual";
        case ::bedrock::protocol::CommandOriginData::Type::GameArgument: return "gameargument";
        case ::bedrock::protocol::CommandOriginData::Type::EntityServer: return "entityserver";
        case ::bedrock::protocol::CommandOriginData::Type::Precompiled: return "precompiled";
        case ::bedrock::protocol::CommandOriginData::Type::GameDirectorEntityServer: return "gamedirectorentityserver";
        case ::bedrock::protocol::CommandOriginData::Type::Scripting: return "scripting";
        case ::bedrock::protocol::CommandOriginData::Type::ExecuteContext: return "executecontext";
        default: return invalid_tag;
    }
}

}
