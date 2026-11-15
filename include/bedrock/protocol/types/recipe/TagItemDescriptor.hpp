#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

#include <bedrock/protocol/types/recipe/ItemDescriptor.hpp>
namespace bedrock::protocol {


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

} // namespace bedrock::protocol
