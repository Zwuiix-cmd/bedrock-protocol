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
#include <bedrock/protocol/types/login/openid/api/MinecraftServiceAuthEnvironment.hpp>
#include <bedrock/protocol/types/login/openid/api/MinecraftServiceAuthEnvironment.hpp>

class MinecraftServiceAuthEnvironments {
public:
    std::shared_ptr<MinecraftServiceAuthEnvironment> prod;

    MinecraftServiceAuthEnvironments() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        prod = ([&]() {
    auto value = std::make_shared<MinecraftServiceAuthEnvironment>();
    value->read(stream);
    return value;
}());
    }

    virtual void write(BinaryStream& stream) const {
        if (prod) prod->write(stream);
    }
};
