#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

#include <bedrock/protocol/types/recipe/ItemDescriptor.hpp>
namespace bedrock::protocol {


class ComplexAliasItemDescriptor : public ItemDescriptor {
public:
    static constexpr int64_t ID = 5;

    std::string alias;

    ComplexAliasItemDescriptor() = default;
    virtual ~ComplexAliasItemDescriptor() = default;
    explicit ComplexAliasItemDescriptor(const std::string& aliasValue) : alias(aliasValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        alias = stream.readString().value_or("");
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(alias);
    }
};

} // namespace bedrock::protocol
