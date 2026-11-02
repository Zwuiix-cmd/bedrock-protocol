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
#include <bedrock/protocol/types/login/openid/api/MinecraftServiceAuthEnvironments.hpp>
#include <bedrock/protocol/types/login/openid/api/MinecraftServiceAuthEnvironments.hpp>

class MinecraftServiceEnvironments {
public:
    std::shared_ptr<MinecraftServiceAuthEnvironments> auth;

    MinecraftServiceEnvironments() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        auth = ([&]() {
    auto value = std::make_shared<MinecraftServiceAuthEnvironments>();
    value->read(stream);
    return value;
}());
    }

    virtual void write(BinaryStream& stream) const {
        if (auth) auth->write(stream);
    }
};
