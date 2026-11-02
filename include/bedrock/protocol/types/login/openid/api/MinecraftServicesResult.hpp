#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>

#include <bedrock/protocol/types/login/openid/api/MinecraftServiceEnvironments.hpp>
namespace bedrock::protocol {


class MinecraftServicesResult {
public:
    std::shared_ptr<MinecraftServiceEnvironments> serviceEnvironments;

    MinecraftServicesResult() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        serviceEnvironments = std::make_shared<MinecraftServiceEnvironments>();
        serviceEnvironments->read(stream);
    }

    virtual void write(BinaryStream& stream) const {
        serviceEnvironments->write(stream);
    }
};

} // namespace bedrock::protocol
