#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>
#include <vector>


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
