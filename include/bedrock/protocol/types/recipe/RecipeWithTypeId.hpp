#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

namespace bedrock::protocol {


class RecipeWithTypeId {
public:
    int32_t typeId;

    RecipeWithTypeId() = default;
    virtual ~RecipeWithTypeId() = default;
    explicit RecipeWithTypeId(const int32_t& typeIdValue) : typeId(typeIdValue) {}

    virtual int32_t getTypeId() const { return {}; }
    virtual void write(BinaryStream& stream) const = 0;
    virtual void read(ReadOnlyBinaryStream& stream) = 0;

    static std::shared_ptr<RecipeWithTypeId> create(ReadOnlyBinaryStream& stream);
};

} // namespace bedrock::protocol
