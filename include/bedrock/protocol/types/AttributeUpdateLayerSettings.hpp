#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>

#include <bedrock/protocol/types/AttributeLayerSettings.hpp>
#include <bedrock/protocol/types/AttributeLayerSyncPayload.hpp>
namespace bedrock::protocol {


class AttributeUpdateLayerSettings : public AttributeLayerSyncPayload {
public:
    static constexpr int64_t ID = 1;

    std::string name;
    uint32_t dimension;
    std::shared_ptr<AttributeLayerSettings> settings;

    AttributeUpdateLayerSettings() = default;
    virtual ~AttributeUpdateLayerSettings() = default;
    explicit AttributeUpdateLayerSettings(
        const std::string& nameValue,
        const uint32_t& dimensionValue,
        const std::shared_ptr<AttributeLayerSettings>& settingsValue
    ) :
        name(nameValue),
        dimension(dimensionValue),
        settings(settingsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        name = stream.readString().value_or("");
        dimension = stream.readUnsignedVarInt().value_or(0);
        settings = std::make_shared<AttributeLayerSettings>();
        settings->read(stream);
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(name);
		stream.writeUnsignedVarInt(dimension);
		settings->write(stream);
    }
};

} // namespace bedrock::protocol
