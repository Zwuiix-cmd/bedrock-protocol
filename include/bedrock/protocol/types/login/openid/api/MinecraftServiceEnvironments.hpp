#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>

#include <bedrock/protocol/types/login/openid/api/MinecraftServiceAuthEnvironments.hpp>
namespace bedrock::protocol {


class MinecraftServiceEnvironments {
public:
    std::shared_ptr<MinecraftServiceAuthEnvironments> auth;

    MinecraftServiceEnvironments() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        auth = std::make_shared<MinecraftServiceAuthEnvironments>();
        auth->read(stream);
    }

    virtual void write(BinaryStream& stream) const {
        auth->write(stream);
    }
};

} // namespace bedrock::protocol
