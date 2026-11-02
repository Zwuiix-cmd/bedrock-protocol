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
#include <bedrock/protocol/types/login/openid/api/MinecraftServiceEnvironments.hpp>
#include <bedrock/protocol/types/login/openid/api/MinecraftServiceEnvironments.hpp>

class MinecraftServicesResult {
public:
    std::shared_ptr<MinecraftServiceEnvironments> serviceEnvironments;

    MinecraftServicesResult() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        serviceEnvironments = ([&]() {
    auto value = std::make_shared<MinecraftServiceEnvironments>();
    value->read(stream);
    return value;
}());
    }

    virtual void write(BinaryStream& stream) const {
        if (serviceEnvironments) serviceEnvironments->write(stream);
    }
};
