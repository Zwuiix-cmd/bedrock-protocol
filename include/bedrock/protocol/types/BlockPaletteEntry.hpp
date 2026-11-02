#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <string>

#include <bedrock/protocol/types/NetworkNbt.hpp>
#include <bedrock/protocol/types/entity/MetadataProperty.hpp>
namespace bedrock::protocol {


class BlockPaletteEntry {
public:
    std::string name;
    std::shared_ptr<CompoundTag> states;

    BlockPaletteEntry() = default;
    explicit BlockPaletteEntry(const std::string& nameValue, const std::shared_ptr<CompoundTag>& statesValue) : name(nameValue), states(statesValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        states = NetworkNbtSerializer::read(stream);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(name);
        NetworkNbtSerializer::write(stream, states);
    }
};

} // namespace bedrock::protocol
