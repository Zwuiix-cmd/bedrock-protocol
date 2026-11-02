#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <string>


class PackSetting {
public:
    std::string name;

    PackSetting() = default;
    virtual ~PackSetting() = default;
    explicit PackSetting(const std::string& nameValue) : name(nameValue) {}

    virtual PackSettingType getTypeId() const = 0;
    virtual void write(BinaryStream& stream) const = 0;
    virtual void read(ReadOnlyBinaryStream& stream) = 0;

    static std::shared_ptr<PackSetting> create(ReadOnlyBinaryStream& stream);
};
