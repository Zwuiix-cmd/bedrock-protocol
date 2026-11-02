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
#include <bedrock/protocol/types/command/ChainedSubCommandData.hpp>
#include <bedrock/protocol/types/command/CommandHardEnum.hpp>
#include <bedrock/protocol/types/command/CommandOverload.hpp>
#include <bedrock/protocol/types/command/CommandPermissions.hpp>
#include <bedrock/protocol/types/command/ChainedSubCommandData.hpp>
#include <bedrock/protocol/types/command/CommandHardEnum.hpp>
#include <bedrock/protocol/types/command/CommandOverload.hpp>
#include <bedrock/protocol/types/command/CommandPermissions.hpp>

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
        permission = magic_enum::enum_cast<CommandPermissions>(stream.readVarInt().value_or(0)).value_or(CommandPermissions::Normal);
        aliases = stream.readOptional([&]() {
	return ([&]() {
	auto value = std::make_shared<CommandHardEnum>();
	value->read(stream);
	return value;
	}());
});
        overloads = stream.readList([&]() {
	return ([&]() {
	auto value = std::make_shared<CommandOverload>();
	value->read(stream);
	return value;
	}());
});
        chainedSubCommandData = stream.readList([&]() {
	return ([&]() {
	auto value = std::make_shared<ChainedSubCommandData>();
	value->read(stream);
	return value;
	}());
});
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(name);
        stream.writeString(description);
        stream.writeInt(flags);
        stream.writeVarInt(magic_enum::enum_integer(permission));
        stream.writeOptional(aliases, [&](const auto& optionalValue) {
	if (optionalValue) optionalValue->write(stream);
});
        stream.writeList(overloads, [&](const auto& item) {
	if (item) item->write(stream);
});
        stream.writeList(chainedSubCommandData, [&](const auto& item) {
	if (item) item->write(stream);
});
    }
};
