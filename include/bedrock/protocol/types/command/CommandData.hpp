#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <bedrock/protocol/types/command/ChainedSubCommandData.hpp>
#include <bedrock/protocol/types/command/CommandHardEnum.hpp>
#include <bedrock/protocol/types/command/CommandOverload.hpp>
#include <bedrock/protocol/types/command/CommandPermissions.hpp>
namespace bedrock::protocol {


class CommandData {
public:
    std::string name;
    std::string description;
    int32_t flags;
    CommandPermissions permission;
    std::optional<std::shared_ptr<CommandHardEnum>> aliases;
    std::vector<std::shared_ptr<CommandOverload>> overloads;
    std::vector<std::shared_ptr<ChainedSubCommandData>> chainedSubCommandData;

    CommandData() = default;
    explicit CommandData(
        const std::string& nameValue,
        const std::string& descriptionValue,
        const int32_t& flagsValue,
        const CommandPermissions& permissionValue,
        const std::optional<std::shared_ptr<CommandHardEnum>>& aliasesValue,
        const std::vector<std::shared_ptr<CommandOverload>>& overloadsValue,
        const std::vector<std::shared_ptr<ChainedSubCommandData>>& chainedSubCommandDataValue
    ) :
        name(nameValue),
        description(descriptionValue),
        flags(flagsValue),
        permission(permissionValue),
        aliases(aliasesValue),
        overloads(overloadsValue),
        chainedSubCommandData(chainedSubCommandDataValue) {

    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        description = stream.readString().value_or("");
        flags = stream.readInt().value_or(0);
        permission = magic_enum::enum_cast<CommandPermissions>(
            stream.readVarInt().value_or(0)
        ).value_or(CommandPermissions::Normal);
        aliases = stream.readOptional([&]() {
            return stream.readObject<CommandHardEnum>();
        });
        overloads = stream.readObjectList<CommandOverload>();
        chainedSubCommandData = stream.readObjectList<ChainedSubCommandData>();
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(name);
        stream.writeString(description);
        stream.writeInt(flags);
        stream.writeVarInt(magic_enum::enum_integer(permission));
        stream.writeOptional(aliases, [&](const auto& optionalValue) {
            optionalValue->write(stream);
        });
        stream.writeList(overloads, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeList(chainedSubCommandData, [&](const auto& item) {
            item->write(stream);
        });
    }
};

} // namespace bedrock::protocol
