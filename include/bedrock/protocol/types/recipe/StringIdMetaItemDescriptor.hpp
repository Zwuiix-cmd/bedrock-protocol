#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <exception>
#include <string>

#include <bedrock/protocol/types/recipe/ItemDescriptor.hpp>

class StringIdMetaItemDescriptor : public ItemDescriptor {
public:
    static constexpr int64_t ID = 4;

    std::string id;
    uint16_t meta;

    StringIdMetaItemDescriptor() = default;
    virtual ~StringIdMetaItemDescriptor() = default;
    explicit StringIdMetaItemDescriptor(const std::string& idValue, const uint16_t& metaValue) : id(idValue), meta(metaValue) {
		if (metaValue < 0) {
			throw std::exception{};
		}
    }

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        id = stream.readString().value_or("");
        meta = stream.readUnsignedShort().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(id);
		stream.writeUnsignedShort(meta);
    }
};
