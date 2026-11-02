#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>
#include <vector>


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
