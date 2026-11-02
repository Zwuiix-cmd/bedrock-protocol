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

class PackSetting {
public:
    std::string name;

    PackSetting() = default;
    virtual ~PackSetting() = default;
    explicit PackSetting(const std::string& nameValue) : name(nameValue) {}

    virtual PackSettingType getTypeId() const = 0;
    virtual void write(BinaryStream& stream) const = 0;
    virtual void read(ReadOnlyBinaryStream& stream) = 0;

    static std::shared_ptr<PackSetting> decode(ReadOnlyBinaryStream& stream);
};
