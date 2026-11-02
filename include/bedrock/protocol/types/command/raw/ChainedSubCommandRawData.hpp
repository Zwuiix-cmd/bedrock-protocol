#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include <bedrock/protocol/types/command/raw/ChainedSubCommandValueRawData.hpp>
namespace bedrock::protocol {


class ChainedSubCommandRawData {
public:
    std::string name;
    std::vector<std::shared_ptr<ChainedSubCommandValueRawData>> valueData;

    ChainedSubCommandRawData() = default;
    explicit ChainedSubCommandRawData(
        const std::string& nameValue,
        const std::vector<std::shared_ptr<ChainedSubCommandValueRawData>>& valueDataValue
    ) :
        name(nameValue),
        valueData(valueDataValue) {}

    static std::shared_ptr<ChainedSubCommandRawData> create(ReadOnlyBinaryStream& stream) {
		auto name = stream.readString().value_or("");
		auto valueData = std::vector<std::shared_ptr<ChainedSubCommandValueRawData>>();
		auto valueDataCount = stream.readUnsignedVarInt().value_or(0);
		const bool valueDataCountAvailable = stream.canReadCollection(valueDataCount);
		for (std::size_t i = 0; i < valueDataCount && valueDataCountAvailable && !stream.hasOverflowed(); i++) {
		    valueData.push_back(stream.readObject<ChainedSubCommandValueRawData>());
		}
		return std::make_shared<ChainedSubCommandRawData>(name, valueData);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		stream.writeUnsignedVarInt(valueData.size());
		for (const auto& valueDatum : valueData) {
			valueDatum->write(stream);
		}
    }
};

} // namespace bedrock::protocol
