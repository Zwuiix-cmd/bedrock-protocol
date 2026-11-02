#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

namespace bedrock::protocol {


class SerializableVoxelCells {
public:
    uint8_t xSize;
    uint8_t ySize;
    uint8_t zSize;
    std::vector<uint8_t> storage;

    SerializableVoxelCells() = default;
    explicit SerializableVoxelCells(
        const uint8_t& xSizeValue,
        const uint8_t& ySizeValue,
        const uint8_t& zSizeValue,
        const std::vector<uint8_t>& storageValue
    ) :
        xSize(xSizeValue),
        ySize(ySizeValue),
        zSize(zSizeValue),
        storage(storageValue) {}

    static std::shared_ptr<SerializableVoxelCells> create(ReadOnlyBinaryStream& stream) {
		auto xSize = stream.readUnsignedByte().value_or(0);
		auto ySize = stream.readUnsignedByte().value_or(0);
		auto zSize = stream.readUnsignedByte().value_or(0);
		auto storage = std::vector<uint8_t>();
		auto storageCount = stream.readUnsignedVarInt().value_or(0);
		const bool storageCountAvailable = stream.canReadCollection(storageCount);
		for (std::size_t i = 0; i < storageCount && storageCountAvailable && !stream.hasOverflowed(); ++i) {
		    storage.push_back(stream.readUnsignedByte().value_or(0));
		}
		return std::make_shared<SerializableVoxelCells>(static_cast<int32_t>(xSize), static_cast<int32_t>(ySize), static_cast<int32_t>(zSize), storage);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
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

} // namespace bedrock::protocol
