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

class StringIdMetaItemDescriptor : public ItemDescriptor {
public:
    std::string id;
    int32_t meta;

    StringIdMetaItemDescriptor() = default;
    virtual ~StringIdMetaItemDescriptor() = default;
    explicit StringIdMetaItemDescriptor(const std::string& idValue, const int32_t& metaValue) : id(idValue), meta(metaValue) {
		if ((metaValue < 0)) {
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
