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

class MolangItemDescriptor : public ItemDescriptor {
public:
    std::string molangExpression;
    uint16_t molangVersion;

    MolangItemDescriptor() = default;
    virtual ~MolangItemDescriptor() = default;
    explicit MolangItemDescriptor(const std::string& molangExpressionValue, const uint16_t& molangVersionValue) : molangExpression(molangExpressionValue), molangVersion(molangVersionValue) {}

    void read(ReadOnlyBinaryStream& stream) override {
        molangExpression = stream.readString().value_or("");
        molangVersion = stream.readUnsignedShort().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(molangExpression);
		stream.writeUnsignedShort(molangVersion);
    }
};
