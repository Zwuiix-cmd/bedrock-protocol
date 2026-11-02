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
#include <bedrock/protocol/types/inventory/NetworkInventoryAction.hpp>
#include <bedrock/protocol/types/inventory/NetworkInventoryAction.hpp>

class TransactionData {
public:
    std::vector<std::shared_ptr<NetworkInventoryAction>> actions;

    TransactionData() = default;
    virtual ~TransactionData() = default;

    virtual int32_t getTypeId() const = 0;
    virtual void read(ReadOnlyBinaryStream& stream) = 0;
    virtual void write(BinaryStream& stream) const = 0;

    static std::shared_ptr<TransactionData> decode(ReadOnlyBinaryStream& stream);
};
