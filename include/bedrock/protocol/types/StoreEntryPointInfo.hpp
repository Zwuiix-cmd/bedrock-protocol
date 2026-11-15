#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

namespace bedrock::protocol {


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

} // namespace bedrock::protocol
