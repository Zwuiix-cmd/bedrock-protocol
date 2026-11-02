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
