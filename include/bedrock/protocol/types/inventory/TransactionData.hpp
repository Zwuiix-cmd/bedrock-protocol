#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/inventory/NetworkInventoryAction.hpp>
namespace bedrock::protocol {


class TransactionData {
public:
    std::vector<std::shared_ptr<NetworkInventoryAction>> actions;

    TransactionData() = default;
    virtual ~TransactionData() = default;

    virtual int32_t getTypeId() const = 0;
    virtual void read(ReadOnlyBinaryStream& stream) = 0;
    virtual void write(BinaryStream& stream) const = 0;

    static std::shared_ptr<TransactionData> create(ReadOnlyBinaryStream& stream);
};

} // namespace bedrock::protocol
