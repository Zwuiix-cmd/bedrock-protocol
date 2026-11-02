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

class ScreenCloseReason {
public:
    static constexpr std::string_view PROGRAMMATIC_CLOSE = "programmaticclose";
    static constexpr std::string_view PROGRAMMATIC_CLOSE_ALL = "programmaticcloseall";
    static constexpr std::string_view CLIENT_CANCELED = "clientcanceled";
    static constexpr std::string_view USER_BUSY = "userbusy";
    static constexpr std::string_view INVALID_FORM = "invalidform";

    ScreenCloseReason() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        (void)stream;
    }

    virtual void write(BinaryStream& stream) const {
        (void)stream;
    }
};
