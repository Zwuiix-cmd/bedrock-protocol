#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>


class CommandSoftEnum {
public:
    std::string name;
    std::vector<std::string> values;

    CommandSoftEnum() = default;
    explicit CommandSoftEnum(const std::string& nameValue, const std::vector<std::string>& valuesValue) : name(nameValue), values(valuesValue) {

    }

    static std::shared_ptr<CommandSoftEnum> create(ReadOnlyBinaryStream& stream) {
		auto name = stream.readString().value_or("");
		auto values = std::vector<std::string>();
		auto valuesCount = stream.readUnsignedVarInt().value_or(0);
		for (std::size_t i = 0; i < valuesCount; i++) {
		    values.push_back(stream.readString().value_or(""));
		}
		return std::make_shared<CommandSoftEnum>(name, values);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		stream.writeUnsignedVarInt(values.size());
		for (const auto& value : values) {
			stream.writeString(value);
		}
    }
};
