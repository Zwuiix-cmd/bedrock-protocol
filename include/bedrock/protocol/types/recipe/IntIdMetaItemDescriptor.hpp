#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <exception>

#include <bedrock/protocol/types/recipe/ItemDescriptor.hpp>

class IntIdMetaItemDescriptor : public ItemDescriptor {
public:
    static constexpr int64_t ID = 1;

    int16_t id;
    int16_t meta;

    IntIdMetaItemDescriptor() = default;
    virtual ~IntIdMetaItemDescriptor() = default;
    explicit IntIdMetaItemDescriptor(const int16_t& idValue, const int16_t& metaValue) : id(idValue), meta(metaValue) {
		if ((idValue == 0) && (metaValue != 0)) {
			throw std::exception{};
		}
    }

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        id = stream.readShort().value_or(0);
        meta = stream.readShort().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeShort(id);
		if (id != 0) {
			stream.writeShort(meta);
		}
    }
};
