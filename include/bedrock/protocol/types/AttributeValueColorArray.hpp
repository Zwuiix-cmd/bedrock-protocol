#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

#include <bedrock/protocol/types/AttributeValueColorValue.hpp>

class AttributeValueColorArray : public AttributeValueColorValue {
public:
    static constexpr int64_t ID = 1;

    uint32_t value;

    AttributeValueColorArray() = default;
    virtual ~AttributeValueColorArray() = default;
    explicit AttributeValueColorArray(const uint32_t& valueValue) : value(valueValue) {}

    static std::shared_ptr<AttributeValueColorArray> create(ReadOnlyBinaryStream& stream) {
		auto r = stream.readUnsignedInt().value_or(0);
		auto g = stream.readUnsignedInt().value_or(0);
		auto b = stream.readUnsignedInt().value_or(0);
		auto a = stream.readUnsignedInt().value_or(0);
		return std::make_shared<AttributeValueColorArray>(static_cast<uint32_t>(((static_cast<uint32_t>(a) << 24) | (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | static_cast<uint32_t>(b))));
    }

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedInt((((value) >> 16) & 0xffu));
		stream.writeUnsignedInt((((value) >> 8) & 0xffu));
		stream.writeUnsignedInt(((value) & 0xffu));
		stream.writeUnsignedInt((((value) >> 24) & 0xffu));
    }
};
