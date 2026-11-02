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
#include <bedrock/protocol/types/StructureSettings.hpp>
#include <bedrock/protocol/types/StructureSettings.hpp>

class StructureEditorData {
public:
    enum class Type : uint8_t {
        Data = 0,
        Save = 1,
        Load = 2,
        Corner = 3,
        Invalid = 4,
        Export = 5,
    };

    std::string structureName;
    std::optional<std::string> filteredStructureName;
    std::string structureDataField;
    bool includePlayers;
    bool showBoundingBox;
    int32_t structureBlockType;
    std::shared_ptr<StructureSettings> structureSettings;
    uint8_t structureRedstoneSaveMode;

    StructureEditorData() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        structureName = stream.readString().value_or("");
        structureDataField = stream.readString().value_or("");
        includePlayers = stream.readBool().value_or(false);
        showBoundingBox = stream.readBool().value_or(false);
        structureBlockType = stream.readVarInt().value_or(0);
        structureRedstoneSaveMode = stream.readUnsignedByte().value_or(0);
        filteredStructureName = stream.readOptional([&]() {
			return stream.readString().value_or("");
		});
        structureSettings = ([&]() {
    auto value = std::make_shared<StructureSettings>();
    value->read(stream);
    return value;
}());
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(structureName);
        stream.writeString(structureDataField);
        stream.writeBool(includePlayers);
        stream.writeBool(showBoundingBox);
        stream.writeVarInt(structureBlockType);
        stream.writeUnsignedByte(structureRedstoneSaveMode);
        stream.writeOptional(filteredStructureName, [&](const auto& optionalValue) {
			stream.writeString(optionalValue);
		});
        if (structureSettings) structureSettings->write(stream);
    }
};
