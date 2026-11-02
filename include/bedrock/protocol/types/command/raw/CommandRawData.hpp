#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <bedrock/protocol/types/command/raw/CommandOverloadRawData.hpp>

class CommandRawData {
public:
    std::string name;
    std::string description;
    uint16_t flags;
    std::string permission;
    int32_t aliasEnumIndex;
    std::vector<uint32_t> chainedSubCommandDataIndexes;
    std::vector<std::shared_ptr<CommandOverloadRawData>> overloads;

    CommandRawData() = default;
    explicit CommandRawData(
        const std::string& nameValue,
        const std::string& descriptionValue,
        const uint16_t& flagsValue,
        const std::string& permissionValue,
        const int32_t& aliasEnumIndexValue,
        const std::vector<uint32_t>& chainedSubCommandDataIndexesValue,
        const std::vector<std::shared_ptr<CommandOverloadRawData>>& overloadsValue
    ) :
        name(nameValue),
        description(descriptionValue),
        flags(flagsValue),
        permission(permissionValue),
        aliasEnumIndex(aliasEnumIndexValue),
        chainedSubCommandDataIndexes(chainedSubCommandDataIndexesValue),
        overloads(overloadsValue) {}

    static std::shared_ptr<CommandRawData> create(ReadOnlyBinaryStream& stream) {
		auto name = stream.readString().value_or("");
		auto description = stream.readString().value_or("");
		auto flags = stream.readUnsignedShort().value_or(0);
		auto permission = stream.readString().value_or("");
		auto aliasEnumIndex = stream.readInt().value_or(0);
		auto chainedSubCommandDataIndexes = std::vector<uint32_t>();
		auto chainedSubCommandDataIndexesCount = stream.readUnsignedVarInt().value_or(0);
		for (std::size_t i = 0; i < chainedSubCommandDataIndexesCount; i++) {
		    chainedSubCommandDataIndexes.push_back(stream.readUnsignedInt().value_or(0));
		}
		auto overloads = std::vector<std::shared_ptr<CommandOverloadRawData>>();
		auto overloadsCount = stream.readUnsignedVarInt().value_or(0);
		for (std::size_t i = 0; i < overloadsCount; i++) {
		    overloads.push_back(stream.readObject<CommandOverloadRawData>());
		}
		return std::make_shared<CommandRawData>(name, description, static_cast<int32_t>(flags), permission, aliasEnumIndex, chainedSubCommandDataIndexes, overloads);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
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
