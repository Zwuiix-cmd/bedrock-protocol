#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <memory>
#include <stdexcept>
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

    static std::shared_ptr<CommandOverloadRawData> create(ReadOnlyBinaryStream& stream) {
		auto chaining = stream.readBool().value_or(false);
		auto parameters = std::vector<std::shared_ptr<CommandParameterRawData>>();
		auto parametersCount = stream.readUnsignedVarInt().value_or(0);
		if (parametersCount > ReadOnlyBinaryStream::MaxCollectionSize) {
		    throw std::runtime_error("protocol collection is too large");
		}
		for (std::size_t i = 0; i < parametersCount; i++) {
		    parameters.push_back(stream.readObject<CommandParameterRawData>());
		}
		return std::make_shared<CommandOverloadRawData>(chaining, parameters);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeBool(chaining);
		stream.writeUnsignedVarInt(parameters.size());
		for (const auto& parameter : parameters) {
			parameter->write(stream);
		}
    }
};
