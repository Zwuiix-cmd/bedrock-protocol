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
#include <variant>
#include <bedrock/protocol/types/command/CommandHardEnum.hpp>
#include <bedrock/protocol/types/command/CommandSoftEnum.hpp>

class CommandParameter {
public:
    static constexpr int64_t FLAG_FORCE_COLLAPSE_ENUM = 1;
    static constexpr int64_t FLAG_HAS_ENUM_CONSTRAINT = 2;

    std::string paramName;
    int32_t paramType;
    bool isOptional;
    int32_t flags = 0;
    std::variant<std::monostate, std::shared_ptr<CommandHardEnum>, std::shared_ptr<CommandSoftEnum>> enum_;
    std::optional<std::string> postfix;

    CommandParameter() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        paramName = stream.readString().value_or("");
        paramType = stream.readInt().value_or(0);
        isOptional = stream.readBool().value_or(false);
        flags = stream.readInt().value_or(0);
        enum_ = ([&]() { auto value = stream.readOptional([&]() { return ([&]() {
    auto value = std::make_shared<CommandHardEnum>();
    value->read(stream);
    return value;
}()); }); return value ? std::variant<std::monostate, std::shared_ptr<CommandHardEnum>, std::shared_ptr<CommandSoftEnum>>{*value} : std::variant<std::monostate, std::shared_ptr<CommandHardEnum>, std::shared_ptr<CommandSoftEnum>>{std::monostate{}}; }());
        postfix = stream.readOptional([&]() {
            return stream.readString().value_or("");
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(paramName);
        stream.writeInt(paramType);
        stream.writeBool(isOptional);
        stream.writeInt(flags);
        std::visit([&](const auto& alternative) { using Alternative = std::remove_cvref_t<decltype(alternative)>; if constexpr (std::is_same_v<Alternative, std::shared_ptr<CommandHardEnum>>) { if (alternative) alternative->write(stream); } else if constexpr (std::is_same_v<Alternative, std::shared_ptr<CommandSoftEnum>>) { if (alternative) alternative->write(stream); } }, enum_);
        stream.writeOptional(postfix, [&](const auto& optionalValue) {
            stream.writeString(optionalValue);
        });
    }
};
