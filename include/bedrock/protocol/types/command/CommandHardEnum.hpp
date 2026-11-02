#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include <bedrock/protocol/types/command/ConstrainedEnumValue.hpp>

class CommandHardEnum {
public:
    std::string name;
    std::vector<std::variant<std::string, std::shared_ptr<ConstrainedEnumValue>>> values;

    CommandHardEnum() = default;
    explicit CommandHardEnum(
        const std::string& nameValue,
        const std::vector<std::variant<std::string, std::shared_ptr<ConstrainedEnumValue>>>& valuesValue
    ) :
        name(nameValue),
        values(valuesValue) {

    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        values = stream.readList([&]() {
            return std::variant<std::string, std::shared_ptr<ConstrainedEnumValue>>{stream.readString().value_or("")};
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(name);
        stream.writeList(values, [&](const auto& item) {
            std::visit([&](const auto& alternative) { using Alternative = std::remove_cvref_t<decltype(alternative)>; if constexpr (std::is_same_v<Alternative, std::string>) { stream.writeString(alternative); } else if constexpr (std::is_same_v<Alternative, std::shared_ptr<ConstrainedEnumValue>>) { alternative->write(stream); } }, item);
        });
    }
};
