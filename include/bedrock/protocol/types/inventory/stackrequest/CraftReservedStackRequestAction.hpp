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
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>

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
