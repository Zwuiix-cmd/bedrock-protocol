#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>

#include <bedrock/protocol/types/login/openid/api/MinecraftServiceAuthEnvironment.hpp>

class MinecraftServiceAuthEnvironments {
public:
    std::shared_ptr<MinecraftServiceAuthEnvironment> prod;

    MinecraftServiceAuthEnvironments() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        prod = std::make_shared<MinecraftServiceAuthEnvironment>();
        prod->read(stream);
    }

    virtual void write(BinaryStream& stream) const {
        prod->write(stream);
    }
};
