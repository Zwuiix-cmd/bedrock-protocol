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

class CommandSoftEnum {
public:
    std::string name;
    std::vector<std::string> values;

    CommandSoftEnum() = default;
    explicit CommandSoftEnum(const std::string& nameValue, const std::vector<std::string>& valuesValue) : name(nameValue), values(valuesValue) {

    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        values = stream.readList([&]() {
	return stream.readString().value_or("");
});
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		stream.writeUnsignedVarInt(values.size());
		for (const auto& value : values) {
			stream.writeString(value);
		}
    }
};
