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
#include <bedrock/protocol/types/inventory/stackrequest/DeprecatedCraftingResultsEntry.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/DeprecatedCraftingResultsEntry.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/DeprecatedCraftingResultsEntry.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>

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

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        iterations = stream.readUnsignedByte().value_or(0);
        results = stream.readList([&]() {
			return ([&]() {
			auto value = std::make_shared<DeprecatedCraftingResultsEntry>();
			value->read(stream);
			return value;
			}());
		});
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(results.size());
		for (const auto& result : results) {
			result->write(stream);
		}
		stream.writeUnsignedByte(iterations);
    }
};
