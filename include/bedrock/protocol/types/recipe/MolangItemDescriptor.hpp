#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

#include <bedrock/protocol/types/recipe/ItemDescriptor.hpp>

class MolangItemDescriptor : public ItemDescriptor {
public:
    static constexpr int64_t ID = 2;

    std::string molangExpression;
    uint8_t molangVersion;

    MolangItemDescriptor() = default;
    virtual ~MolangItemDescriptor() = default;
    explicit MolangItemDescriptor(const std::string& molangExpressionValue, const uint8_t& molangVersionValue) : molangExpression(molangExpressionValue), molangVersion(molangVersionValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        molangExpression = stream.readString().value_or("");
        molangVersion = stream.readUnsignedByte().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(molangExpression);
		stream.writeUnsignedByte(molangVersion);
    }
};
