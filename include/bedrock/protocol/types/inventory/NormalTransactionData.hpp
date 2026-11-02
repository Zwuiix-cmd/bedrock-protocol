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
#include <bedrock/protocol/types/inventory/TransactionData.hpp>

class NormalTransactionData : public TransactionData {
public:
    static constexpr int64_t ID = 0;


    NormalTransactionData() = default;
    virtual ~NormalTransactionData() = default;

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        (void)stream;
    }

    void write(BinaryStream& stream) const override {
        (void)stream;
    }
};
