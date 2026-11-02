#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>
namespace bedrock::protocol {


class LoomStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 17;

    std::string patternId;
    uint8_t repetitions = 1;

    LoomStackRequestAction() = default;
    virtual ~LoomStackRequestAction() = default;
    explicit LoomStackRequestAction(const std::string& patternIdValue, const uint8_t& repetitionsValue) : patternId(patternIdValue), repetitions(repetitionsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        patternId = stream.readString().value_or("");
        repetitions = stream.readUnsignedByte().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(patternId);
		stream.writeUnsignedByte(repetitions);
    }
};

} // namespace bedrock::protocol
