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
	std::visit([&](const auto& alternative) { using Alternative = std::remove_cvref_t<decltype(alternative)>; if constexpr (std::is_same_v<Alternative, std::string>) { stream.writeString(alternative); } else if constexpr (std::is_same_v<Alternative, std::shared_ptr<ConstrainedEnumValue>>) { if (alternative) alternative->write(stream); } }, item);
});
    }
};
