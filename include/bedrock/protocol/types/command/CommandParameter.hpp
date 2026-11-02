#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>

#include <bedrock/protocol/types/command/CommandHardEnum.hpp>
#include <bedrock/protocol/types/command/CommandSoftEnum.hpp>

class CommandParameter {
public:
    enum class Flag : uint8_t {
        ForceCollapseEnum = 1,
        HasEnumConstraint = 2,
    };

    std::string paramName;
    int32_t paramType;
    bool isOptional;
    Flag flags = Flag{};
    std::variant<std::monostate, std::shared_ptr<CommandHardEnum>, std::shared_ptr<CommandSoftEnum>> enum_;
    std::optional<std::string> postfix;

    CommandParameter() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        paramName = stream.readString().value_or("");
        paramType = stream.readInt().value_or(0);
        isOptional = stream.readBool().value_or(false);
        flags = static_cast<Flag>(stream.readInt().value_or(0));
        enum_ = ([&]() { auto value = stream.readOptional([&]() { return stream.readObject<CommandHardEnum>(); }); return value ? std::variant<std::monostate, std::shared_ptr<CommandHardEnum>, std::shared_ptr<CommandSoftEnum>>{*value} : std::variant<std::monostate, std::shared_ptr<CommandHardEnum>, std::shared_ptr<CommandSoftEnum>>{std::monostate{}}; }());
        postfix = stream.readOptional([&]() {
            return stream.readString().value_or("");
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(paramName);
        stream.writeInt(paramType);
        stream.writeBool(isOptional);
        stream.writeInt(magic_enum::enum_integer(flags));
        std::visit([&](const auto& alternative) { using Alternative = std::remove_cvref_t<decltype(alternative)>; if constexpr (std::is_same_v<Alternative, std::shared_ptr<CommandHardEnum>>) { alternative->write(stream); } else if constexpr (std::is_same_v<Alternative, std::shared_ptr<CommandSoftEnum>>) { alternative->write(stream); } }, enum_);
        stream.writeOptional(postfix, [&](const auto& optionalValue) {
            stream.writeString(optionalValue);
        });
    }
};
