#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include <bedrock/protocol/types/inventory/NetworkItemDescriptor.hpp>
#include <bedrock/protocol/types/recipe/RecipeWithTypeId.hpp>

class FurnaceRecipe : public RecipeWithTypeId {
public:
    int32_t inputId;
    std::optional<int32_t> inputMeta;
    NetworkItemDescriptor result;
    std::string blockName;

    FurnaceRecipe() = default;
    virtual ~FurnaceRecipe() = default;
    explicit FurnaceRecipe(
        const int32_t& typeIdValue,
        const int32_t& inputIdValue,
        const std::optional<int32_t>& inputMetaValue,
        const NetworkItemDescriptor& resultValue,
        const std::string& blockNameValue
    ) :
        RecipeWithTypeId(typeIdValue),
        inputId(inputIdValue),
        inputMeta(inputMetaValue),
        result(resultValue),
        blockName(blockNameValue) {}

    static std::shared_ptr<FurnaceRecipe> create(const int32_t& typeId, ReadOnlyBinaryStream& stream) {
        auto result = std::make_shared<FurnaceRecipe>();
        result->inputId = stream.readVarInt().value_or(0);
        result->inputMeta = stream.readOptional([&]() {
            return stream.readVarInt().value_or(0);
        });
        result->result = stream.readValue<NetworkItemDescriptor>();
        result->blockName = stream.readString().value_or("");
        return result;
    }

    void read(ReadOnlyBinaryStream& stream) override {
        inputId = stream.readVarInt().value_or(0);
        inputMeta = stream.readOptional([&]() {
            return stream.readVarInt().value_or(0);
        });
        result = stream.readValue<NetworkItemDescriptor>();
        blockName = stream.readString().value_or("");
    }

    void write(BinaryStream& stream) const override {
        stream.writeVarInt(inputId);
        stream.writeOptional(inputMeta, [&](const auto& optionalValue) {
            stream.writeVarInt(optionalValue);
        });
        result.write(stream);
        stream.writeString(blockName);
    }
};
