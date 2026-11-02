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
#include <bedrock/protocol/types/command/ChainedSubCommandValue.hpp>
#include <bedrock/protocol/types/command/ChainedSubCommandValue.hpp>

class ChainedSubCommandData {
public:
    std::string name;
    std::vector<std::shared_ptr<ChainedSubCommandValue>> values;

    ChainedSubCommandData() = default;
    explicit ChainedSubCommandData(
        const std::string& nameValue,
        const std::vector<std::shared_ptr<ChainedSubCommandValue>>& valuesValue
    ) :
        name(nameValue),
        values(valuesValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        values = stream.readList([&]() {
	return ([&]() {
	auto value = std::make_shared<ChainedSubCommandValue>();
	value->read(stream);
	return value;
	}());
});
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(name);
        stream.writeList(values, [&](const auto& item) {
	if (item) item->write(stream);
});
    }
};
