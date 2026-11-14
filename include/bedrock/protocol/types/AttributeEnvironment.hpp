#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <string>

#include <bedrock/protocol/types/AttributeEnvironmentPayload.hpp>
namespace bedrock::protocol {


class AttributeEnvironment {
public:
    std::string name;
    std::shared_ptr<AttributeEnvironmentPayload> payload;

    AttributeEnvironment() = default;
    explicit AttributeEnvironment(const std::string& nameValue, const std::shared_ptr<AttributeEnvironmentPayload>& payloadValue) : name(nameValue), payload(payloadValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        payload = AttributeEnvironmentPayload::create(stream);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		stream.writeUnsignedVarInt(payload->getTypeId());
		payload->write(stream);
    }
};

} // namespace bedrock::protocol
