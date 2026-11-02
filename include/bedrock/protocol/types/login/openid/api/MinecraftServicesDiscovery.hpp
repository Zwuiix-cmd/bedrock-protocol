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
