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
#include <bedrock/protocol/types/AttributeEnvironmentPayload.hpp>

class AttributeEnvironment {
public:
    std::string name;
    std::shared_ptr<AttributeEnvironmentPayload> payload;

    AttributeEnvironment() = default;
    explicit AttributeEnvironment(const std::string& nameValue, const std::shared_ptr<AttributeEnvironmentPayload>& payloadValue) : name(nameValue), payload(payloadValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        payload = AttributeEnvironmentPayload::decode(stream);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		stream.writeUnsignedVarInt(payload->getTypeId());
		payload->write(stream);
    }
};
