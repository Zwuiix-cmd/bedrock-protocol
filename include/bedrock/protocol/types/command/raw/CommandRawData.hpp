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
#include <bedrock/protocol/types/command/raw/CommandOverloadRawData.hpp>
#include <bedrock/protocol/types/command/raw/CommandOverloadRawData.hpp>

class CommandRawData {
public:
    std::string name;
    std::string description;
    uint16_t flags;
    std::string permission;
    int32_t aliasEnumIndex;
    std::vector<int32_t> chainedSubCommandDataIndexes;
    std::vector<std::shared_ptr<CommandOverloadRawData>> overloads;

    CommandRawData() = default;
    explicit CommandRawData(
        const std::string& nameValue,
        const std::string& descriptionValue,
        const uint16_t& flagsValue,
        const std::string& permissionValue,
        const int32_t& aliasEnumIndexValue,
        const std::vector<int32_t>& chainedSubCommandDataIndexesValue,
        const std::vector<std::shared_ptr<CommandOverloadRawData>>& overloadsValue
    ) :
        name(nameValue),
        description(descriptionValue),
        flags(flagsValue),
        permission(permissionValue),
        aliasEnumIndex(aliasEnumIndexValue),
        chainedSubCommandDataIndexes(chainedSubCommandDataIndexesValue),
        overloads(overloadsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        description = stream.readString().value_or("");
        flags = stream.readUnsignedShort().value_or(0);
        permission = stream.readString().value_or("");
        aliasEnumIndex = stream.readInt().value_or(0);
        chainedSubCommandDataIndexes = stream.readList([&]() {
	return stream.readInt().value_or(0);
});
        overloads = stream.readList([&]() {
	return ([&]() {
	auto value = std::make_shared<CommandOverloadRawData>();
	value->read(stream);
	return value;
	}());
});
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		stream.writeString(description);
		stream.writeUnsignedShort(flags);
		stream.writeString(permission);
		stream.writeInt(aliasEnumIndex);
		stream.writeUnsignedVarInt(chainedSubCommandDataIndexes.size());
		for (const auto& index : chainedSubCommandDataIndexes) {
			stream.writeUnsignedInt(index);
		}
		stream.writeUnsignedVarInt(overloads.size());
		for (const auto& overload : overloads) {
			overload->write(stream);
		}
    }
};
