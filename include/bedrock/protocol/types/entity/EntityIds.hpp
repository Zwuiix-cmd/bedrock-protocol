#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class EntityIds : uint16_t {
    Agent = 0,
    Allay = 1,
    AreaEffectCloud = 2,
    Armadillo = 3,
    ArmorStand = 4,
    Arrow = 5,
    Axolotl = 6,
    Balloon = 7,
    Bat = 8,
    Bee = 9,
    Blaze = 10,
    Boat = 11,
    Bogged = 12,
    Breeze = 13,
    BreezeWindChargeProjectile = 14,
    Camel = 15,
    CamelHusk = 16,
    Cat = 17,
    CaveSpider = 18,
    ChestBoat = 19,
    ChestMinecart = 20,
    Chicken = 21,
    Cod = 22,
    CommandBlockMinecart = 23,
    CopperGolem = 24,
    Cow = 25,
    Creaking = 26,
    Creeper = 27,
    Cushion = 28,
    Dolphin = 29,
    Donkey = 30,
    DragonFireball = 31,
    Drowned = 32,
    Egg = 33,
    ElderGuardian = 34,
    ElderGuardianGhost = 35,
    EnderCrystal = 36,
    EnderDragon = 37,
    EnderPearl = 38,
    Enderman = 39,
    Endermite = 40,
    EvocationFang = 41,
    EvocationIllager = 42,
    EyeOfEnderSignal = 43,
    FallingBlock = 44,
    Fireball = 45,
    FireworksRocket = 46,
    FishingHook = 47,
    Fox = 48,
    Frog = 49,
    Frostbite = 50,
    Ghast = 51,
    GlowSquid = 52,
    Goat = 53,
    Guardian = 54,
    HappyGhast = 55,
    Hoglin = 56,
    HopperMinecart = 57,
    Horse = 58,
    Husk = 59,
    IceBall = 60,
    IceBomb = 61,
    IronGolem = 62,
    Item = 63,
    LeashKnot = 64,
    LightningBolt = 65,
    LingeringPotion = 66,
    Llama = 67,
    LlamaSpit = 68,
    MagmaCube = 69,
    Minecart = 70,
    Mooshroom = 71,
    Mule = 72,
    Nautilus = 73,
    Npc = 74,
    Ocelot = 75,
    OminousItemSpawner = 76,
    Painting = 77,
    Panda = 78,
    Parched = 79,
    Parrot = 80,
    Phantom = 81,
    Pig = 82,
    Piglin = 83,
    PiglinBrute = 84,
    Pillager = 85,
    Player = 86,
    PolarBear = 87,
    Pufferfish = 88,
    Rabbit = 89,
    Ravager = 90,
    Salmon = 91,
    Sheep = 92,
    Shulker = 93,
    ShulkerBullet = 94,
    Silverfish = 95,
    Skeleton = 96,
    SkeletonHorse = 97,
    Slime = 98,
    SmallFireball = 99,
    Sniffer = 100,
    SnowGolem = 101,
    Snowball = 102,
    Spider = 103,
    SplashPotion = 104,
    Squid = 105,
    Stray = 106,
    Strider = 107,
    SulfurCube = 108,
    Tadpole = 109,
    ThrownTrident = 110,
    Tnt = 111,
    TntMinecart = 112,
    TraderLlama = 113,
    TripodCamera = 114,
    Tropicalfish = 115,
    Turtle = 116,
    Vex = 117,
    Villager = 118,
    VillagerV2 = 119,
    Vindicator = 120,
    WanderingTrader = 121,
    Warden = 122,
    WindChargeProjectile = 123,
    Witch = 124,
    Wither = 125,
    WitherSkeleton = 126,
    WitherSkull = 127,
    WitherSkullDangerous = 128,
    Wolf = 129,
    XpBottle = 130,
    XpOrb = 131,
    Zoglin = 132,
    Zombie = 133,
    ZombieHorse = 134,
    ZombieNautilus = 135,
    ZombiePigman = 136,
    ZombieVillager = 137,
    ZombieVillagerV2 = 138,
};

} // namespace bedrock::protocol
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::EntityIds>(::bedrock::protocol::EntityIds value) noexcept {
    switch (value) {
        case ::bedrock::protocol::EntityIds::Agent: return "minecraft:agent";
        case ::bedrock::protocol::EntityIds::Allay: return "minecraft:allay";
        case ::bedrock::protocol::EntityIds::AreaEffectCloud: return "minecraft:area_effect_cloud";
        case ::bedrock::protocol::EntityIds::Armadillo: return "minecraft:armadillo";
        case ::bedrock::protocol::EntityIds::ArmorStand: return "minecraft:armor_stand";
        case ::bedrock::protocol::EntityIds::Arrow: return "minecraft:arrow";
        case ::bedrock::protocol::EntityIds::Axolotl: return "minecraft:axolotl";
        case ::bedrock::protocol::EntityIds::Balloon: return "minecraft:balloon";
        case ::bedrock::protocol::EntityIds::Bat: return "minecraft:bat";
        case ::bedrock::protocol::EntityIds::Bee: return "minecraft:bee";
        case ::bedrock::protocol::EntityIds::Blaze: return "minecraft:blaze";
        case ::bedrock::protocol::EntityIds::Boat: return "minecraft:boat";
        case ::bedrock::protocol::EntityIds::Bogged: return "minecraft:bogged";
        case ::bedrock::protocol::EntityIds::Breeze: return "minecraft:breeze";
        case ::bedrock::protocol::EntityIds::BreezeWindChargeProjectile: return "minecraft:breeze_wind_charge_projectile";
        case ::bedrock::protocol::EntityIds::Camel: return "minecraft:camel";
        case ::bedrock::protocol::EntityIds::CamelHusk: return "minecraft:camel_husk";
        case ::bedrock::protocol::EntityIds::Cat: return "minecraft:cat";
        case ::bedrock::protocol::EntityIds::CaveSpider: return "minecraft:cave_spider";
        case ::bedrock::protocol::EntityIds::ChestBoat: return "minecraft:chest_boat";
        case ::bedrock::protocol::EntityIds::ChestMinecart: return "minecraft:chest_minecart";
        case ::bedrock::protocol::EntityIds::Chicken: return "minecraft:chicken";
        case ::bedrock::protocol::EntityIds::Cod: return "minecraft:cod";
        case ::bedrock::protocol::EntityIds::CommandBlockMinecart: return "minecraft:command_block_minecart";
        case ::bedrock::protocol::EntityIds::CopperGolem: return "minecraft:copper_golem";
        case ::bedrock::protocol::EntityIds::Cow: return "minecraft:cow";
        case ::bedrock::protocol::EntityIds::Creaking: return "minecraft:creaking";
        case ::bedrock::protocol::EntityIds::Creeper: return "minecraft:creeper";
        case ::bedrock::protocol::EntityIds::Cushion: return "minecraft:cushion";
        case ::bedrock::protocol::EntityIds::Dolphin: return "minecraft:dolphin";
        case ::bedrock::protocol::EntityIds::Donkey: return "minecraft:donkey";
        case ::bedrock::protocol::EntityIds::DragonFireball: return "minecraft:dragon_fireball";
        case ::bedrock::protocol::EntityIds::Drowned: return "minecraft:drowned";
        case ::bedrock::protocol::EntityIds::Egg: return "minecraft:egg";
        case ::bedrock::protocol::EntityIds::ElderGuardian: return "minecraft:elder_guardian";
        case ::bedrock::protocol::EntityIds::ElderGuardianGhost: return "minecraft:elder_guardian_ghost";
        case ::bedrock::protocol::EntityIds::EnderCrystal: return "minecraft:ender_crystal";
        case ::bedrock::protocol::EntityIds::EnderDragon: return "minecraft:ender_dragon";
        case ::bedrock::protocol::EntityIds::EnderPearl: return "minecraft:ender_pearl";
        case ::bedrock::protocol::EntityIds::Enderman: return "minecraft:enderman";
        case ::bedrock::protocol::EntityIds::Endermite: return "minecraft:endermite";
        case ::bedrock::protocol::EntityIds::EvocationFang: return "minecraft:evocation_fang";
        case ::bedrock::protocol::EntityIds::EvocationIllager: return "minecraft:evocation_illager";
        case ::bedrock::protocol::EntityIds::EyeOfEnderSignal: return "minecraft:eye_of_ender_signal";
        case ::bedrock::protocol::EntityIds::FallingBlock: return "minecraft:falling_block";
        case ::bedrock::protocol::EntityIds::Fireball: return "minecraft:fireball";
        case ::bedrock::protocol::EntityIds::FireworksRocket: return "minecraft:fireworks_rocket";
        case ::bedrock::protocol::EntityIds::FishingHook: return "minecraft:fishing_hook";
        case ::bedrock::protocol::EntityIds::Fox: return "minecraft:fox";
        case ::bedrock::protocol::EntityIds::Frog: return "minecraft:frog";
        case ::bedrock::protocol::EntityIds::Frostbite: return "minecraft:frostbite";
        case ::bedrock::protocol::EntityIds::Ghast: return "minecraft:ghast";
        case ::bedrock::protocol::EntityIds::GlowSquid: return "minecraft:glow_squid";
        case ::bedrock::protocol::EntityIds::Goat: return "minecraft:goat";
        case ::bedrock::protocol::EntityIds::Guardian: return "minecraft:guardian";
        case ::bedrock::protocol::EntityIds::HappyGhast: return "minecraft:happy_ghast";
        case ::bedrock::protocol::EntityIds::Hoglin: return "minecraft:hoglin";
        case ::bedrock::protocol::EntityIds::HopperMinecart: return "minecraft:hopper_minecart";
        case ::bedrock::protocol::EntityIds::Horse: return "minecraft:horse";
        case ::bedrock::protocol::EntityIds::Husk: return "minecraft:husk";
        case ::bedrock::protocol::EntityIds::IceBall: return "minecraft:ice_ball";
        case ::bedrock::protocol::EntityIds::IceBomb: return "minecraft:ice_bomb";
        case ::bedrock::protocol::EntityIds::IronGolem: return "minecraft:iron_golem";
        case ::bedrock::protocol::EntityIds::Item: return "minecraft:item";
        case ::bedrock::protocol::EntityIds::LeashKnot: return "minecraft:leash_knot";
        case ::bedrock::protocol::EntityIds::LightningBolt: return "minecraft:lightning_bolt";
        case ::bedrock::protocol::EntityIds::LingeringPotion: return "minecraft:lingering_potion";
        case ::bedrock::protocol::EntityIds::Llama: return "minecraft:llama";
        case ::bedrock::protocol::EntityIds::LlamaSpit: return "minecraft:llama_spit";
        case ::bedrock::protocol::EntityIds::MagmaCube: return "minecraft:magma_cube";
        case ::bedrock::protocol::EntityIds::Minecart: return "minecraft:minecart";
        case ::bedrock::protocol::EntityIds::Mooshroom: return "minecraft:mooshroom";
        case ::bedrock::protocol::EntityIds::Mule: return "minecraft:mule";
        case ::bedrock::protocol::EntityIds::Nautilus: return "minecraft:nautilus";
        case ::bedrock::protocol::EntityIds::Npc: return "minecraft:npc";
        case ::bedrock::protocol::EntityIds::Ocelot: return "minecraft:ocelot";
        case ::bedrock::protocol::EntityIds::OminousItemSpawner: return "minecraft:ominous_item_spawner";
        case ::bedrock::protocol::EntityIds::Painting: return "minecraft:painting";
        case ::bedrock::protocol::EntityIds::Panda: return "minecraft:panda";
        case ::bedrock::protocol::EntityIds::Parched: return "minecraft:parched";
        case ::bedrock::protocol::EntityIds::Parrot: return "minecraft:parrot";
        case ::bedrock::protocol::EntityIds::Phantom: return "minecraft:phantom";
        case ::bedrock::protocol::EntityIds::Pig: return "minecraft:pig";
        case ::bedrock::protocol::EntityIds::Piglin: return "minecraft:piglin";
        case ::bedrock::protocol::EntityIds::PiglinBrute: return "minecraft:piglin_brute";
        case ::bedrock::protocol::EntityIds::Pillager: return "minecraft:pillager";
        case ::bedrock::protocol::EntityIds::Player: return "minecraft:player";
        case ::bedrock::protocol::EntityIds::PolarBear: return "minecraft:polar_bear";
        case ::bedrock::protocol::EntityIds::Pufferfish: return "minecraft:pufferfish";
        case ::bedrock::protocol::EntityIds::Rabbit: return "minecraft:rabbit";
        case ::bedrock::protocol::EntityIds::Ravager: return "minecraft:ravager";
        case ::bedrock::protocol::EntityIds::Salmon: return "minecraft:salmon";
        case ::bedrock::protocol::EntityIds::Sheep: return "minecraft:sheep";
        case ::bedrock::protocol::EntityIds::Shulker: return "minecraft:shulker";
        case ::bedrock::protocol::EntityIds::ShulkerBullet: return "minecraft:shulker_bullet";
        case ::bedrock::protocol::EntityIds::Silverfish: return "minecraft:silverfish";
        case ::bedrock::protocol::EntityIds::Skeleton: return "minecraft:skeleton";
        case ::bedrock::protocol::EntityIds::SkeletonHorse: return "minecraft:skeleton_horse";
        case ::bedrock::protocol::EntityIds::Slime: return "minecraft:slime";
        case ::bedrock::protocol::EntityIds::SmallFireball: return "minecraft:small_fireball";
        case ::bedrock::protocol::EntityIds::Sniffer: return "minecraft:sniffer";
        case ::bedrock::protocol::EntityIds::SnowGolem: return "minecraft:snow_golem";
        case ::bedrock::protocol::EntityIds::Snowball: return "minecraft:snowball";
        case ::bedrock::protocol::EntityIds::Spider: return "minecraft:spider";
        case ::bedrock::protocol::EntityIds::SplashPotion: return "minecraft:splash_potion";
        case ::bedrock::protocol::EntityIds::Squid: return "minecraft:squid";
        case ::bedrock::protocol::EntityIds::Stray: return "minecraft:stray";
        case ::bedrock::protocol::EntityIds::Strider: return "minecraft:strider";
        case ::bedrock::protocol::EntityIds::SulfurCube: return "minecraft:sulfur_cube";
        case ::bedrock::protocol::EntityIds::Tadpole: return "minecraft:tadpole";
        case ::bedrock::protocol::EntityIds::ThrownTrident: return "minecraft:thrown_trident";
        case ::bedrock::protocol::EntityIds::Tnt: return "minecraft:tnt";
        case ::bedrock::protocol::EntityIds::TntMinecart: return "minecraft:tnt_minecart";
        case ::bedrock::protocol::EntityIds::TraderLlama: return "minecraft:trader_llama";
        case ::bedrock::protocol::EntityIds::TripodCamera: return "minecraft:tripod_camera";
        case ::bedrock::protocol::EntityIds::Tropicalfish: return "minecraft:tropicalfish";
        case ::bedrock::protocol::EntityIds::Turtle: return "minecraft:turtle";
        case ::bedrock::protocol::EntityIds::Vex: return "minecraft:vex";
        case ::bedrock::protocol::EntityIds::Villager: return "minecraft:villager";
        case ::bedrock::protocol::EntityIds::VillagerV2: return "minecraft:villager_v2";
        case ::bedrock::protocol::EntityIds::Vindicator: return "minecraft:vindicator";
        case ::bedrock::protocol::EntityIds::WanderingTrader: return "minecraft:wandering_trader";
        case ::bedrock::protocol::EntityIds::Warden: return "minecraft:warden";
        case ::bedrock::protocol::EntityIds::WindChargeProjectile: return "minecraft:wind_charge_projectile";
        case ::bedrock::protocol::EntityIds::Witch: return "minecraft:witch";
        case ::bedrock::protocol::EntityIds::Wither: return "minecraft:wither";
        case ::bedrock::protocol::EntityIds::WitherSkeleton: return "minecraft:wither_skeleton";
        case ::bedrock::protocol::EntityIds::WitherSkull: return "minecraft:wither_skull";
        case ::bedrock::protocol::EntityIds::WitherSkullDangerous: return "minecraft:wither_skull_dangerous";
        case ::bedrock::protocol::EntityIds::Wolf: return "minecraft:wolf";
        case ::bedrock::protocol::EntityIds::XpBottle: return "minecraft:xp_bottle";
        case ::bedrock::protocol::EntityIds::XpOrb: return "minecraft:xp_orb";
        case ::bedrock::protocol::EntityIds::Zoglin: return "minecraft:zoglin";
        case ::bedrock::protocol::EntityIds::Zombie: return "minecraft:zombie";
        case ::bedrock::protocol::EntityIds::ZombieHorse: return "minecraft:zombie_horse";
        case ::bedrock::protocol::EntityIds::ZombieNautilus: return "minecraft:zombie_nautilus";
        case ::bedrock::protocol::EntityIds::ZombiePigman: return "minecraft:zombie_pigman";
        case ::bedrock::protocol::EntityIds::ZombieVillager: return "minecraft:zombie_villager";
        case ::bedrock::protocol::EntityIds::ZombieVillagerV2: return "minecraft:zombie_villager_v2";
        default: return invalid_tag;
    }
}

}
