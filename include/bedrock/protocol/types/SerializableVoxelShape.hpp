#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/SerializableVoxelCells.hpp>

class SerializableVoxelShape {
public:
    std::shared_ptr<SerializableVoxelCells> cells;
    std::vector<float> xCoordinates;
    std::vector<float> yCoordinates;
    std::vector<float> zCoordinates;

    SerializableVoxelShape() = default;
    explicit SerializableVoxelShape(
        const std::shared_ptr<SerializableVoxelCells>& cellsValue,
        const std::vector<float>& xCoordinatesValue,
        const std::vector<float>& yCoordinatesValue,
        const std::vector<float>& zCoordinatesValue
    ) :
        cells(cellsValue),
        xCoordinates(xCoordinatesValue),
        yCoordinates(yCoordinatesValue),
        zCoordinates(zCoordinatesValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        cells = std::make_shared<SerializableVoxelCells>();
        cells->read(stream);
        xCoordinates = stream.readList([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        yCoordinates = stream.readList([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        zCoordinates = stream.readList([&]() {
            return stream.readFloat().value_or(0.0f);
        });
    }

    virtual void write(BinaryStream& stream) const {
		cells->write(stream);
		stream.writeUnsignedVarInt(xCoordinates.size());
		for (const auto& value : xCoordinates) {
			stream.writeFloat(value);
		}
		stream.writeUnsignedVarInt(yCoordinates.size());
		for (const auto& value : yCoordinates) {
			stream.writeFloat(value);
		}
		stream.writeUnsignedVarInt(zCoordinates.size());
		for (const auto& value : zCoordinates) {
			stream.writeFloat(value);
		}
    }
};
