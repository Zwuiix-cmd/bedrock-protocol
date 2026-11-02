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
#include <bedrock/protocol/types/recipe/RecipeWithTypeId.hpp>

class MultiRecipe : public RecipeWithTypeId {
public:
    static constexpr std::string_view TYPE_REPAIR_ITEM = "00000000-0000-0000-0000-000000000001";
    static constexpr std::string_view TYPE_MAP_EXTENDING = "D392B075-4BA1-40AE-8789-AF868D56F6CE";
    static constexpr std::string_view TYPE_MAP_EXTENDING_CARTOGRAPHY = "8B36268C-1829-483C-A0F1-993B7156A8F2";
    static constexpr std::string_view TYPE_MAP_CLONING = "85939755-BA10-4D9D-A4CC-EFB7A8E943C4";
    static constexpr std::string_view TYPE_MAP_CLONING_CARTOGRAPHY = "442D85ED-8272-4543-A6F1-418F90DED05D";
    static constexpr std::string_view TYPE_MAP_UPGRADING = "AECD2294-4B94-434B-8667-4499BB2C9327";
    static constexpr std::string_view TYPE_MAP_UPGRADING_CARTOGRAPHY = "98C84B38-1085-46BD-B1CE-DD38C159E6CC";
    static constexpr std::string_view TYPE_BOOK_CLONING = "D1CA6B84-338E-4F2F-9C6B-76CC8B4BD98D";
    static constexpr std::string_view TYPE_BANNER_DUPLICATE = "B5C5D105-75A2-4076-AF2B-923EA2BF4BF0";
    static constexpr std::string_view TYPE_BANNER_ADD_PATTERN = "D81AAEAF-E172-4440-9225-868DF030D27B";
    static constexpr std::string_view TYPE_FIREWORKS = "00000000-0000-0000-0000-000000000002";
    static constexpr std::string_view TYPE_MAP_LOCKING_CARTOGRAPHY = "602234E4-CAC1-4353-8BB7-B1EBFF70024B";
    std::string recipeId;
    int32_t recipeNetId;

    MultiRecipe() = default;
    virtual ~MultiRecipe() = default;
    explicit MultiRecipe(const int32_t& typeIdValue, const std::string& recipeIdValue, const int32_t& recipeNetIdValue) : RecipeWithTypeId(typeIdValue), recipeId(recipeIdValue), recipeNetId(recipeNetIdValue) {
		(void)0;
    }

    static std::shared_ptr<MultiRecipe> decode(const int32_t& typeId, ReadOnlyBinaryStream& stream) {
		auto uuid = stream.readUUID().value_or("");
		auto recipeNetId = stream.readUnsignedVarInt().value_or(0);
		return std::make_shared<MultiRecipe>(typeId, uuid, recipeNetId);
    }

    void read(ReadOnlyBinaryStream& stream) override {
        recipeId = stream.readUUID().value_or("");
        recipeNetId = stream.readInt().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUUID(recipeId);
		stream.writeUnsignedVarInt(recipeNetId);
    }
};
