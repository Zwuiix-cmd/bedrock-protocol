#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <memory>
#include <string>

#include <bedrock/protocol/types/StructureSettings.hpp>
namespace bedrock::protocol {


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
    std::string filteredStructureName;
    std::string structureDataField;
    bool includePlayers;
    bool showBoundingBox;
    Type structureBlockType;
    std::shared_ptr<StructureSettings> structureSettings;
    int32_t structureRedstoneSaveMode;

    StructureEditorData() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        structureName = stream.readString().value_or("");
        filteredStructureName = stream.readString().value_or("");
        structureDataField = stream.readString().value_or("");
        includePlayers = stream.readBool().value_or(false);
        showBoundingBox = stream.readBool().value_or(false);
        structureBlockType = magic_enum::enum_cast<Type>(
            stream.readVarInt().value_or(0)
        ).value_or(Type::Data);
        structureRedstoneSaveMode = stream.readVarInt().value_or(0);
        structureSettings = std::make_shared<StructureSettings>();
        structureSettings->read(stream);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(structureName);
        stream.writeString(filteredStructureName);
        stream.writeString(structureDataField);
        stream.writeBool(includePlayers);
        stream.writeBool(showBoundingBox);
        stream.writeVarInt(magic_enum::enum_integer(structureBlockType));
        stream.writeVarInt(structureRedstoneSaveMode);
        structureSettings->write(stream);
    }
};

} // namespace bedrock::protocol
