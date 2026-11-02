#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <exception>

#include <bedrock/protocol/types/inventory/TransactionData.hpp>
namespace bedrock::protocol {


class MismatchTransactionData : public TransactionData {
public:
    static constexpr int64_t ID = 1;


    MismatchTransactionData() = default;
    virtual ~MismatchTransactionData() = default;

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        actions = stream.readObjectList<NetworkInventoryAction>();
        if (actions.size() > 0) {
            throw std::exception{};
        }
    }

    void write(BinaryStream& stream) const override {
        stream.writeList(actions, [&](const auto& item) {
            item->write(stream);
        });
    }
};

} // namespace bedrock::protocol
