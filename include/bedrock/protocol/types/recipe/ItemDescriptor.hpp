#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>

namespace bedrock::protocol {


class ItemDescriptor {
public:

    ItemDescriptor() = default;
    virtual ~ItemDescriptor() = default;
    virtual void write(BinaryStream& stream) const = 0;
    virtual void read(ReadOnlyBinaryStream& stream) = 0;

    static std::shared_ptr<ItemDescriptor> create(ReadOnlyBinaryStream& stream);
};

} // namespace bedrock::protocol
