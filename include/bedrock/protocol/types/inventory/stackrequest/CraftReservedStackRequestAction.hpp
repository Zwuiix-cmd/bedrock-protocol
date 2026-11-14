#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>
namespace bedrock::protocol {


class CraftReservedStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 16;

    std::string reservedId;
    uint8_t numCrafts;

    CraftReservedStackRequestAction() = default;
    virtual ~CraftReservedStackRequestAction() = default;
    explicit CraftReservedStackRequestAction(const std::string& reservedIdValue, const uint8_t& numCraftsValue) : reservedId(reservedIdValue), numCrafts(numCraftsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        reservedId = stream.readString().value_or("");
        numCrafts = stream.readUnsignedByte().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(reservedId);
		stream.writeUnsignedByte(numCrafts);
    }
};

} // namespace bedrock::protocol
