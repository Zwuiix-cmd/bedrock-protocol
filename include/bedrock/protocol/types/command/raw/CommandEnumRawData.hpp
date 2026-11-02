#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>


class CommandEnumRawData {
public:
    std::string name;
    std::vector<uint32_t> valueIndexes;

    CommandEnumRawData() = default;
    explicit CommandEnumRawData(const std::string& nameValue, const std::vector<uint32_t>& valueIndexesValue) : name(nameValue), valueIndexes(valueIndexesValue) {}

    static std::shared_ptr<CommandEnumRawData> create(ReadOnlyBinaryStream& stream) {
		auto name = stream.readString().value_or("");
		auto valueIndexes = std::vector<uint32_t>();
		auto size = stream.readUnsignedVarInt().value_or(0);
		    const bool sizeAvailable = stream.canReadCollection(size);
		for (std::size_t i = 0; (i < size) && !stream.hasOverflowed() && sizeAvailable && !stream.hasOverflowed(); i++) {
			valueIndexes.push_back(stream.readUnsignedInt().value_or(0));
		}
		return std::make_shared<CommandEnumRawData>(name, valueIndexes);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		stream.writeUnsignedVarInt(valueIndexes.size());
		for (const auto& index : valueIndexes) {
			stream.writeUnsignedInt(index);
		}
    }
};
