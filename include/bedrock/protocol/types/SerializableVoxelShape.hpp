#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/SerializableVoxelCells.hpp>
namespace bedrock::protocol {


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

    static std::shared_ptr<SerializableVoxelShape> create(ReadOnlyBinaryStream& stream) {
		auto cells = std::make_shared<SerializableVoxelCells>();
		cells->read(stream);
		auto xCoordinates = std::vector<float>();
		auto xCoordinatesCount = stream.readUnsignedVarInt().value_or(0);
		const bool xCoordinatesCountAvailable = stream.canReadCollection(xCoordinatesCount);
		for (std::size_t i = 0; i < xCoordinatesCount && xCoordinatesCountAvailable && !stream.hasOverflowed(); ++i) {
		    xCoordinates.push_back(stream.readFloat().value_or(0.0f));
		}
		auto yCoordinates = std::vector<float>();
		auto yCoordinatesCount = stream.readUnsignedVarInt().value_or(0);
		const bool yCoordinatesCountAvailable = stream.canReadCollection(yCoordinatesCount);
		for (std::size_t i = 0; i < yCoordinatesCount && yCoordinatesCountAvailable && !stream.hasOverflowed(); ++i) {
		    yCoordinates.push_back(stream.readFloat().value_or(0.0f));
		}
		auto zCoordinates = std::vector<float>();
		auto zCoordinatesCount = stream.readUnsignedVarInt().value_or(0);
		const bool zCoordinatesCountAvailable = stream.canReadCollection(zCoordinatesCount);
		for (std::size_t i = 0; i < zCoordinatesCount && zCoordinatesCountAvailable && !stream.hasOverflowed(); ++i) {
		    zCoordinates.push_back(stream.readFloat().value_or(0.0f));
		}
		return std::make_shared<SerializableVoxelShape>(cells, xCoordinates, yCoordinates, zCoordinates);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
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

} // namespace bedrock::protocol
