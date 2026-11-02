#pragma once

#include <array>
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
        type = magic_enum::enum_cast<CommandOriginData::Type>(std::string_view(stream.readString().value_or(""))).value_or(CommandOriginData::Type::Player);
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

inline constexpr std::array<std::string_view, 16> CommandOriginDataTypeNames = {
    "player",
    "commandblock",
    "minecartcommandblock",
    "devconsole",
    "test",
    "automationplayer",
    "clientautomation",
    "dedicatedserver",
    "entity",
    "virtual",
    "gameargument",
    "entityserver",
    "precompiled",
    "gamedirectorentityserver",
    "scripting",
    "executecontext",
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<CommandOriginData::Type>(CommandOriginData::Type value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < CommandOriginDataTypeNames.size() ? customize_t{CommandOriginDataTypeNames[index]} : invalid_tag;
}

}
