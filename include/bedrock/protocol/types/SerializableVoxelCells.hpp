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

class SerializableVoxelCells {
public:
    uint8_t xSize;
    uint8_t ySize;
    uint8_t zSize;
    std::vector<int32_t> storage;

    SerializableVoxelCells() = default;
    explicit SerializableVoxelCells(
        const uint8_t& xSizeValue,
        const uint8_t& ySizeValue,
        const uint8_t& zSizeValue,
        const std::vector<int32_t>& storageValue
    ) :
        xSize(xSizeValue),
        ySize(ySizeValue),
        zSize(zSizeValue),
        storage(storageValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        xSize = stream.readUnsignedByte().value_or(0);
        ySize = stream.readUnsignedByte().value_or(0);
        zSize = stream.readUnsignedByte().value_or(0);
        storage = stream.readList([&]() {
	return stream.readInt().value_or(0);
});
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(xSize);
		stream.writeUnsignedByte(ySize);
		stream.writeUnsignedByte(zSize);
		stream.writeUnsignedVarInt(storage.size());
		for (const auto& value : storage) {
			stream.writeUnsignedByte(value);
		}
    }
};
