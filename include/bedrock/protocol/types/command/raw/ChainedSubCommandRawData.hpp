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
#include <bedrock/protocol/types/command/raw/ChainedSubCommandValueRawData.hpp>

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

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        valueData = stream.readObjectList<ChainedSubCommandValueRawData>();
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		stream.writeUnsignedVarInt(valueData.size());
		for (const auto& valueDatum : valueData) {
			valueDatum->write(stream);
		}
    }
};
