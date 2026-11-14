#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

namespace bedrock::protocol {


class ClientStoreEntrypointConfig {
public:
    std::string storeId;
    std::string storeName;

    ClientStoreEntrypointConfig() = default;
    explicit ClientStoreEntrypointConfig(const std::string& storeIdValue, const std::string& storeNameValue) : storeId(storeIdValue), storeName(storeNameValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        storeId = stream.readString().value_or("");
        storeName = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(storeId);
		stream.writeString(storeName);
    }
};

} // namespace bedrock::protocol
