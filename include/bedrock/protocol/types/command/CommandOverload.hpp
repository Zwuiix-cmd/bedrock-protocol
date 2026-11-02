#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/command/CommandParameter.hpp>

class CommandOverload {
public:
    bool chaining;
    std::vector<std::shared_ptr<CommandParameter>> parameters;

    CommandOverload() = default;
    explicit CommandOverload(const bool& chainingValue, const std::vector<std::shared_ptr<CommandParameter>>& parametersValue) : chaining(chainingValue), parameters(parametersValue) {

    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        chaining = stream.readBool().value_or(false);
        parameters = stream.readObjectList<CommandParameter>();
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeBool(chaining);
        stream.writeList(parameters, [&](const auto& item) {
            item->write(stream);
        });
    }
};
