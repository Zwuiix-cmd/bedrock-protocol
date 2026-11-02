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
            NetworkItemDescriptor value;
            value.read(stream);
            return value;
        });
    }

    void write(BinaryStream& stream) const override {
        stream.writeUnsignedByte(iterations);
        stream.writeList(results, [&](const auto& item) {
            item.write(stream);
        });
    }
};
