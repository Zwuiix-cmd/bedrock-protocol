#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/command/raw/CommandParameterRawData.hpp>

class CommandOverloadRawData {
public:
    bool chaining;
    std::vector<std::shared_ptr<CommandParameterRawData>> parameters;

    CommandOverloadRawData() = default;
    explicit CommandOverloadRawData(
        const bool& chainingValue,
        const std::vector<std::shared_ptr<CommandParameterRawData>>& parametersValue
    ) :
        chaining(chainingValue),
        parameters(parametersValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        chaining = stream.readBool().value_or(false);
        parameters = stream.readObjectList<CommandParameterRawData>();
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeBool(chaining);
		stream.writeUnsignedVarInt(parameters.size());
		for (const auto& parameter : parameters) {
			parameter->write(stream);
		}
    }
};
