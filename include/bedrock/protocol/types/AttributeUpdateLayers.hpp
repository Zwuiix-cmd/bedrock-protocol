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
#include <bedrock/protocol/types/AttributeLayer.hpp>
#include <bedrock/protocol/types/AttributeLayerSyncPayload.hpp>

class AttributeUpdateLayers : public AttributeLayerSyncPayload {
public:
    static constexpr int64_t ID = 0;

    std::vector<std::shared_ptr<AttributeLayer>> layers;

    AttributeUpdateLayers() = default;
    virtual ~AttributeUpdateLayers() = default;
    explicit AttributeUpdateLayers(const std::vector<std::shared_ptr<AttributeLayer>>& layersValue) : layers(layersValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        layers = stream.readObjectList<AttributeLayer>();
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(layers.size());
		for (const auto& layer : layers) {
			layer->write(stream);
		}
    }
};
