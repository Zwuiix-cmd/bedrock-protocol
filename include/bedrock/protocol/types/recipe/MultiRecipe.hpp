#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <memory>
#include <string>

#include <bedrock/protocol/types/recipe/RecipeWithTypeId.hpp>

class MultiRecipe : public RecipeWithTypeId {
public:
    enum class Type : uint8_t {
        RepairItem = 0,
        MapExtending = 1,
        MapExtendingCartography = 2,
        MapCloning = 3,
        MapCloningCartography = 4,
        MapUpgrading = 5,
        MapUpgradingCartography = 6,
        BookCloning = 7,
        BannerDuplicate = 8,
        BannerAddPattern = 9,
        Fireworks = 10,
        MapLockingCartography = 11,
    };

    std::string recipeId;
    int32_t recipeNetId;

    MultiRecipe() = default;
    virtual ~MultiRecipe() = default;
    explicit MultiRecipe(const int32_t& typeIdValue, const std::string& recipeIdValue, const int32_t& recipeNetIdValue) : RecipeWithTypeId(typeIdValue), recipeId(recipeIdValue), recipeNetId(recipeNetIdValue) {}

    static std::shared_ptr<MultiRecipe> create(const int32_t& typeId, ReadOnlyBinaryStream& stream) {
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

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<MultiRecipe::Type>(MultiRecipe::Type value) noexcept {
    switch (value) {
        case MultiRecipe::Type::RepairItem: return "00000000-0000-0000-0000-000000000001";
        case MultiRecipe::Type::MapExtending: return "D392B075-4BA1-40AE-8789-AF868D56F6CE";
        case MultiRecipe::Type::MapExtendingCartography: return "8B36268C-1829-483C-A0F1-993B7156A8F2";
        case MultiRecipe::Type::MapCloning: return "85939755-BA10-4D9D-A4CC-EFB7A8E943C4";
        case MultiRecipe::Type::MapCloningCartography: return "442D85ED-8272-4543-A6F1-418F90DED05D";
        case MultiRecipe::Type::MapUpgrading: return "AECD2294-4B94-434B-8667-4499BB2C9327";
        case MultiRecipe::Type::MapUpgradingCartography: return "98C84B38-1085-46BD-B1CE-DD38C159E6CC";
        case MultiRecipe::Type::BookCloning: return "D1CA6B84-338E-4F2F-9C6B-76CC8B4BD98D";
        case MultiRecipe::Type::BannerDuplicate: return "B5C5D105-75A2-4076-AF2B-923EA2BF4BF0";
        case MultiRecipe::Type::BannerAddPattern: return "D81AAEAF-E172-4440-9225-868DF030D27B";
        case MultiRecipe::Type::Fireworks: return "00000000-0000-0000-0000-000000000002";
        case MultiRecipe::Type::MapLockingCartography: return "602234E4-CAC1-4353-8BB7-B1EBFF70024B";
        default: return invalid_tag;
    }
}

}
