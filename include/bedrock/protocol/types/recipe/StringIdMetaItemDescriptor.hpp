#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <exception>
#include <string>

#include <bedrock/protocol/types/recipe/ItemDescriptor.hpp>
namespace bedrock::protocol {


class StringIdMetaItemDescriptor : public ItemDescriptor {
public:
    std::string id;
    int32_t meta;

    StringIdMetaItemDescriptor() = default;
    virtual ~StringIdMetaItemDescriptor() = default;
    explicit StringIdMetaItemDescriptor(const std::string& idValue, const int32_t& metaValue) : id(idValue), meta(metaValue) {
		if (metaValue < 0) {
			throw std::exception{};
		}
    }

    void read(ReadOnlyBinaryStream& stream) override {
        id = stream.readString().value_or("");
        meta = stream.readVarInt().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(id);
		stream.writeVarInt(meta);
    }
};

} // namespace bedrock::protocol
