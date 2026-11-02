#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/inventory/NetworkItemDescriptor.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>
namespace bedrock::protocol {


class DeprecatedCraftingResultsStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 19;

    std::vector<NetworkItemDescriptor> results;
    uint8_t iterations;

    DeprecatedCraftingResultsStackRequestAction() = default;
    virtual ~DeprecatedCraftingResultsStackRequestAction() = default;
    explicit DeprecatedCraftingResultsStackRequestAction(const std::vector<NetworkItemDescriptor>& resultsValue, const uint8_t& iterationsValue) : results(resultsValue), iterations(iterationsValue) {}

    static std::shared_ptr<DeprecatedCraftingResultsStackRequestAction> create(ReadOnlyBinaryStream& stream) {
        auto result = std::make_shared<DeprecatedCraftingResultsStackRequestAction>();
        result->results = stream.readList([&]() {
            return stream.readValue<NetworkItemDescriptor>();
        });
        result->iterations = stream.readUnsignedByte().value_or(0);
        return result;
    }

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    void write(BinaryStream& stream) const override {
        stream.writeList(results, [&](const auto& item) {
            item.write(stream);
        });
        stream.writeUnsignedByte(iterations);
    }
};

} // namespace bedrock::protocol
