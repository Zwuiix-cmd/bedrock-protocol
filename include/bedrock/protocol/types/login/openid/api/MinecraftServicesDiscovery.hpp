#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>

#include <bedrock/protocol/types/login/openid/api/MinecraftServicesResult.hpp>

class MinecraftServicesDiscovery {
public:
    std::shared_ptr<MinecraftServicesResult> result;

    MinecraftServicesDiscovery() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        result = std::make_shared<MinecraftServicesResult>();
        result->read(stream);
    }

    virtual void write(BinaryStream& stream) const {
        result->write(stream);
    }
};
