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

class CommandEnumRawData {
public:
    std::string name;
    std::vector<int32_t> valueIndexes;

    CommandEnumRawData() = default;
    explicit CommandEnumRawData(const std::string& nameValue, const std::vector<int32_t>& valueIndexesValue) : name(nameValue), valueIndexes(valueIndexesValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        valueIndexes = stream.readList([&]() {
            return stream.readInt().value_or(0);
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		stream.writeUnsignedVarInt(valueIndexes.size());
		for (const auto& index : valueIndexes) {
			stream.writeUnsignedInt(index);
		}
    }
};
