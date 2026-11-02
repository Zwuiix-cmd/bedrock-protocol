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

class RecipeWithTypeId {
public:
    int32_t typeId;

    RecipeWithTypeId() = default;
    virtual ~RecipeWithTypeId() = default;
    explicit RecipeWithTypeId(const int32_t& typeIdValue) : typeId(typeIdValue) {}

    virtual int32_t getTypeId() const { return {}; }
    virtual void write(BinaryStream& stream) const = 0;
    virtual void read(ReadOnlyBinaryStream& stream) = 0;

    static std::shared_ptr<RecipeWithTypeId> decode(ReadOnlyBinaryStream& stream);
};
