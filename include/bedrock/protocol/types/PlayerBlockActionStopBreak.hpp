#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

namespace bedrock::protocol {


class PlayerBlockActionStopBreak {
public:

    PlayerBlockActionStopBreak() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        (void)stream;
    }

    virtual void write(BinaryStream& stream) const {
        (void)stream;
    }
};

} // namespace bedrock::protocol
