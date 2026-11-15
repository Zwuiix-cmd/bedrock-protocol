#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

namespace bedrock::protocol {


class ItemStackRequestAction {
public:

    ItemStackRequestAction() = default;
    virtual ~ItemStackRequestAction() = default;

    virtual int32_t getTypeId() const = 0;
    virtual void write(BinaryStream& stream) const = 0;
    virtual void read(ReadOnlyBinaryStream& stream) = 0;

    static std::shared_ptr<ItemStackRequestAction> create(ReadOnlyBinaryStream& stream);
    static std::shared_ptr<ItemStackRequestAction> create(ReadOnlyBinaryStream& stream, uint32_t type);
};

} // namespace bedrock::protocol
