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
#include <bedrock/protocol/types/NetworkNbt.hpp>
#include <bedrock/protocol/types/entity/MetadataProperty.hpp>

class ItemStackExtraData {
public:
    std::optional<std::shared_ptr<CompoundTag>> nbt;
    std::vector<std::string> canPlaceOn;
    std::vector<std::string> canDestroy;

    ItemStackExtraData() = default;
    explicit ItemStackExtraData(
        const std::optional<std::shared_ptr<CompoundTag>>& nbtValue,
        const std::vector<std::string>& canPlaceOnValue,
        const std::vector<std::string>& canDestroyValue
    ) :
        nbt(nbtValue),
        canPlaceOn(canPlaceOnValue),
        canDestroy(canDestroyValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        nbt = stream.readOptional([&]() {
            return NetworkNbtSerializer::read(stream);
        });
        canPlaceOn = stream.readList([&]() {
            return stream.readString().value_or("");
        });
        canDestroy = stream.readList([&]() {
            return stream.readString().value_or("");
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeOptional(nbt, [&](const auto& optionalValue) {
            NetworkNbtSerializer::write(stream, optionalValue);
        });
        stream.writeList(canPlaceOn, [&](const auto& item) {
            stream.writeString(item);
        });
        stream.writeList(canDestroy, [&](const auto& item) {
            stream.writeString(item);
        });
    }
};
