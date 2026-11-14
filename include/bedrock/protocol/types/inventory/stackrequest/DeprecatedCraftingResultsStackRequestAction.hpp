#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/inventory/stackrequest/DeprecatedCraftingResultsEntry.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>
namespace bedrock::protocol {


class DeprecatedCraftingResultsStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 18;

    std::vector<std::shared_ptr<DeprecatedCraftingResultsEntry>> results;
    uint8_t iterations;

    DeprecatedCraftingResultsStackRequestAction() = default;
    virtual ~DeprecatedCraftingResultsStackRequestAction() = default;
    explicit DeprecatedCraftingResultsStackRequestAction(
        const std::vector<std::shared_ptr<DeprecatedCraftingResultsEntry>>& resultsValue,
        const uint8_t& iterationsValue
    ) :
        results(resultsValue),
        iterations(iterationsValue) {}

    static std::shared_ptr<DeprecatedCraftingResultsStackRequestAction> create(ReadOnlyBinaryStream& stream) {
		auto results = std::vector<std::shared_ptr<DeprecatedCraftingResultsEntry>>();
		auto resultsCount = stream.readUnsignedVarInt().value_or(0);
		const bool resultsCountAvailable = stream.canReadCollection(resultsCount);
		for (std::size_t i = 0; i < resultsCount && resultsCountAvailable && !stream.hasOverflowed(); ++i) {
		    results.push_back(stream.readObject<DeprecatedCraftingResultsEntry>());
		}
		auto iterations = stream.readUnsignedByte().value_or(0);
		return std::make_shared<DeprecatedCraftingResultsStackRequestAction>(results, static_cast<int32_t>(iterations));
    }

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(results.size());
		for (const auto& result : results) {
			result->write(stream);
		}
		stream.writeUnsignedByte(iterations);
    }
};

} // namespace bedrock::protocol
