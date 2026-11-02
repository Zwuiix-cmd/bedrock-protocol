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

class TagItemDescriptor : public ItemDescriptor {
public:
    static constexpr int64_t ID = 3;

    std::string tag;

    TagItemDescriptor() = default;
    virtual ~TagItemDescriptor() = default;
    explicit TagItemDescriptor(const std::string& tagValue) : tag(tagValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        tag = stream.readString().value_or("");
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(tag);
    }
};
