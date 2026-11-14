#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

namespace bedrock::protocol {


class DynamicValue {
public:

    DynamicValue() = default;
    virtual ~DynamicValue() = default;

    virtual int32_t getTypeId() const = 0;
    virtual void read(ReadOnlyBinaryStream& stream) = 0;
    virtual void write(BinaryStream& stream) const = 0;

    static std::shared_ptr<DynamicValue> create(ReadOnlyBinaryStream& stream);
};

} // namespace bedrock::protocol
