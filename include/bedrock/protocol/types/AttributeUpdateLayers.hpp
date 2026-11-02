#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/AttributeLayer.hpp>
#include <bedrock/protocol/types/AttributeLayerSyncPayload.hpp>

class AttributeUpdateLayers : public AttributeLayerSyncPayload {
public:
    static constexpr int64_t ID = 0;

    std::vector<std::shared_ptr<AttributeLayer>> layers;

    AttributeUpdateLayers() = default;
    virtual ~AttributeUpdateLayers() = default;
    explicit AttributeUpdateLayers(const std::vector<std::shared_ptr<AttributeLayer>>& layersValue) : layers(layersValue) {}

    static std::shared_ptr<AttributeUpdateLayers> create(ReadOnlyBinaryStream& stream) {
		auto layers = std::vector<std::shared_ptr<AttributeLayer>>();
		auto layersCount = stream.readUnsignedVarInt().value_or(0);
		const bool layersCountAvailable = stream.canReadCollection(layersCount);
		for (std::size_t i = 0; i < layersCount && layersCountAvailable && !stream.hasOverflowed(); ++i) {
		    layers.push_back(stream.readObject<AttributeLayer>());
		}
		return std::make_shared<AttributeUpdateLayers>(layers);
    }

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(layers.size());
		for (const auto& layer : layers) {
			layer->write(stream);
		}
    }
};
