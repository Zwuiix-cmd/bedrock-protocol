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
#include <bedrock/protocol/types/recipe/ItemDescriptor.hpp>

class TagItemDescriptor : public ItemDescriptor {
public:
    static constexpr int64_t DEFAULT_META = 32767;

    std::string tag;
    int32_t meta = 32767;

    TagItemDescriptor() = default;
    virtual ~TagItemDescriptor() = default;
    explicit TagItemDescriptor(const std::string& tagValue, const int32_t& metaValue) : tag(tagValue), meta(metaValue) {}

    void read(ReadOnlyBinaryStream& stream) override {
        tag = stream.readString().value_or("");
        meta = stream.readVarInt().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(tag);
		stream.writeVarInt(meta);
    }
};
