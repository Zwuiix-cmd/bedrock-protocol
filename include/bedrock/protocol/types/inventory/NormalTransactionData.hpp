#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/inventory/TransactionData.hpp>
namespace bedrock::protocol {


class NormalTransactionData : public TransactionData {
public:
    static constexpr int64_t ID = 0;


    NormalTransactionData() = default;
    virtual ~NormalTransactionData() = default;

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        actions = stream.readObjectList<NetworkInventoryAction>();
    }

    void write(BinaryStream& stream) const override {
        stream.writeList(actions, [&](const auto& item) {
            item->write(stream);
        });
    }
};

} // namespace bedrock::protocol
