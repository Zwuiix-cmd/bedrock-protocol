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

class LegacyAuthChain {
public:
    std::vector<std::string> chain;

    LegacyAuthChain() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        chain = stream.readList([&]() {
	return stream.readString().value_or("");
});
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeList(chain, [&](const auto& item) {
	stream.writeString(item);
});
    }
};
