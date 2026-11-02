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

class StoreEntryPointInfo {
public:
    std::string id;
    std::string name;

    StoreEntryPointInfo() = default;
    explicit StoreEntryPointInfo(const std::string& idValue, const std::string& nameValue) : id(idValue), name(nameValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        id = stream.readString().value_or("");
        name = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(id);
		stream.writeString(name);
    }
};
