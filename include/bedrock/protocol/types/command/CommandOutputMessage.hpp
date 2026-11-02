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

class CommandOutputMessage {
public:
    bool isInternal;
    std::string messageId;
    std::vector<std::string> parameters;

    CommandOutputMessage() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        messageId = stream.readString().value_or("");
        isInternal = stream.readBool().value_or(false);
        parameters = stream.readList([&]() {
	return stream.readString().value_or("");
});
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(messageId);
        stream.writeBool(isInternal);
        stream.writeList(parameters, [&](const auto& item) {
	stream.writeString(item);
});
    }
};
