<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
    void write(BinaryStream& stream) override {
        stream.writeActorUniqueID(mapId);
        stream.writeUnsignedByte(dimensionId);
        stream.writeBool(isLocked);
        stream.writeSignedBlockPos(origin);
        stream.writeOptional(scale, [&](const auto& optionalValue) {
            stream.writeUnsignedByte(optionalValue);
        });
        stream.writeOptional(xOffset, [&](const auto& optionalValue) {
            stream.writeVarInt(optionalValue);
        });
        stream.writeOptional(yOffset, [&](const auto& optionalValue) {
            stream.writeVarInt(optionalValue);
        });
        stream.writeList(parentMapIds, [&](const auto& item) {
            stream.writeInt(item);
        });
        stream.writeList(trackedEntities, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeList(decorations, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeOptional(colors, [&](const auto& optionalValue) {
            optionalValue->write(stream);
        });
    }
BEFORE,
<<<'AFTER'
    void write(BinaryStream& stream) override {
        stream.writeActorUniqueID(mapId);
        stream.writeUnsignedByte(dimensionId);
        stream.writeBool(isLocked);
        stream.writeSignedBlockPos(origin);
        stream.writeOptional(parentMapIds, [&](const auto& ids) {
            stream.writeList(ids, [&](int64_t id) { stream.writeActorUniqueID(id); });
        });
        stream.writeOptional(scale, [&](uint8_t value) { stream.writeUnsignedByte(value); });
        stream.writeOptional(trackedEntities, [&](const auto& entities) {
            stream.writeList(entities, [&](const auto& entity) { entity->write(stream); });
        });
        stream.writeOptional(decorations, [&](const auto& values) {
            stream.writeList(values, [&](const auto& value) { value->write(stream); });
        });

        const auto width = colors ? std::optional{(*colors)->width} : std::nullopt;
        const auto height = colors ? std::optional{(*colors)->height} : std::nullopt;
        const auto writeDimension = [&](int32_t value) { stream.writeVarInt(value); };
        stream.writeOptional(width, writeDimension);
        stream.writeOptional(height, writeDimension);
        stream.writeOptional(xOffset, writeDimension);
        stream.writeOptional(yOffset, writeDimension);
        stream.writeOptional(colors, [&](const auto& image) {
            stream.writeUnsignedVarInt(image->width * image->height);
            image->write(stream);
        });
    }
AFTER],
    [<<<'BEFORE'
std::optional<std::vector<int32_t>> parentMapIds;
BEFORE,
<<<'AFTER'
std::optional<std::vector<int64_t>> parentMapIds;
AFTER],
];
