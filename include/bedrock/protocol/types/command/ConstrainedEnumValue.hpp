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

class ConstrainedEnumValue {
public:
    enum class Requires : uint8_t {
        CheatsEnabled = 1,
        ElevatedPermissions = 2,
        HostPermissions = 4,
        AllowAliases = 8,
    };

    std::string value;
    std::vector<int32_t> constraints;

    ConstrainedEnumValue() = default;
    explicit ConstrainedEnumValue(const std::string& valueValue, const std::vector<int32_t>& constraintsValue) : value(valueValue), constraints(constraintsValue) {

    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        value = stream.readString().value_or("");
        constraints = stream.readList([&]() {
            return stream.readInt().value_or(0);
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(value);
        stream.writeList(constraints, [&](const auto& item) {
            stream.writeInt(item);
        });
    }
};
