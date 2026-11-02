#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <vector>

#include <bedrock/protocol/types/inventory/NetworkItemDescriptor.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>

class DeprecatedCraftingResultsStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 19;

    std::vector<NetworkItemDescriptor> results;
    uint8_t iterations;

    DeprecatedCraftingResultsStackRequestAction() = default;
    virtual ~DeprecatedCraftingResultsStackRequestAction() = default;
    explicit DeprecatedCraftingResultsStackRequestAction(const std::vector<NetworkItemDescriptor>& resultsValue, const uint8_t& iterationsValue) : results(resultsValue), iterations(iterationsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        iterations = stream.readUnsignedByte().value_or(0);
        results = stream.readList([&]() {
            return stream.readValue<NetworkItemDescriptor>();
        });
    }

    void write(BinaryStream& stream) const override {
        stream.writeUnsignedByte(iterations);
        stream.writeList(results, [&](const auto& item) {
            item.write(stream);
        });
    }
};
