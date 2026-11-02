#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

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

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<EntityIds>(EntityIds value) noexcept {
    switch (value) {
        case EntityIds::Agent: return "minecraft:agent";
        case EntityIds::Allay: return "minecraft:allay";
        case EntityIds::AreaEffectCloud: return "minecraft:area_effect_cloud";
        case EntityIds::Armadillo: return "minecraft:armadillo";
        case EntityIds::ArmorStand: return "minecraft:armor_stand";
        case EntityIds::Arrow: return "minecraft:arrow";
        case EntityIds::Axolotl: return "minecraft:axolotl";
        case EntityIds::Balloon: return "minecraft:balloon";
        case EntityIds::Bat: return "minecraft:bat";
        case EntityIds::Bee: return "minecraft:bee";
        case EntityIds::Blaze: return "minecraft:blaze";
        case EntityIds::Boat: return "minecraft:boat";
        case EntityIds::Bogged: return "minecraft:bogged";
        case EntityIds::Breeze: return "minecraft:breeze";
        case EntityIds::BreezeWindChargeProjectile: return "minecraft:breeze_wind_charge_projectile";
        case EntityIds::Camel: return "minecraft:camel";
        case EntityIds::CamelHusk: return "minecraft:camel_husk";
        case EntityIds::Cat: return "minecraft:cat";
        case EntityIds::CaveSpider: return "minecraft:cave_spider";
        case EntityIds::ChestBoat: return "minecraft:chest_boat";
        case EntityIds::ChestMinecart: return "minecraft:chest_minecart";
        case EntityIds::Chicken: return "minecraft:chicken";
        case EntityIds::Cod: return "minecraft:cod";
        case EntityIds::CommandBlockMinecart: return "minecraft:command_block_minecart";
        case EntityIds::CopperGolem: return "minecraft:copper_golem";
        case EntityIds::Cow: return "minecraft:cow";
        case EntityIds::Creaking: return "minecraft:creaking";
        case EntityIds::Creeper: return "minecraft:creeper";
        case EntityIds::Cushion: return "minecraft:cushion";
        case EntityIds::Dolphin: return "minecraft:dolphin";
        case EntityIds::Donkey: return "minecraft:donkey";
        case EntityIds::DragonFireball: return "minecraft:dragon_fireball";
        case EntityIds::Drowned: return "minecraft:drowned";
        case EntityIds::Egg: return "minecraft:egg";
        case EntityIds::ElderGuardian: return "minecraft:elder_guardian";
        case EntityIds::ElderGuardianGhost: return "minecraft:elder_guardian_ghost";
        case EntityIds::EnderCrystal: return "minecraft:ender_crystal";
        case EntityIds::EnderDragon: return "minecraft:ender_dragon";
        case EntityIds::EnderPearl: return "minecraft:ender_pearl";
        case EntityIds::Enderman: return "minecraft:enderman";
        case EntityIds::Endermite: return "minecraft:endermite";
        case EntityIds::EvocationFang: return "minecraft:evocation_fang";
        case EntityIds::EvocationIllager: return "minecraft:evocation_illager";
        case EntityIds::EyeOfEnderSignal: return "minecraft:eye_of_ender_signal";
        case EntityIds::FallingBlock: return "minecraft:falling_block";
        case EntityIds::Fireball: return "minecraft:fireball";
        case EntityIds::FireworksRocket: return "minecraft:fireworks_rocket";
        case EntityIds::FishingHook: return "minecraft:fishing_hook";
        case EntityIds::Fox: return "minecraft:fox";
        case EntityIds::Frog: return "minecraft:frog";
        case EntityIds::Frostbite: return "minecraft:frostbite";
        case EntityIds::Ghast: return "minecraft:ghast";
        case EntityIds::GlowSquid: return "minecraft:glow_squid";
        case EntityIds::Goat: return "minecraft:goat";
        case EntityIds::Guardian: return "minecraft:guardian";
        case EntityIds::HappyGhast: return "minecraft:happy_ghast";
        case EntityIds::Hoglin: return "minecraft:hoglin";
        case EntityIds::HopperMinecart: return "minecraft:hopper_minecart";
        case EntityIds::Horse: return "minecraft:horse";
        case EntityIds::Husk: return "minecraft:husk";
        case EntityIds::IceBall: return "minecraft:ice_ball";
        case EntityIds::IceBomb: return "minecraft:ice_bomb";
        case EntityIds::IronGolem: return "minecraft:iron_golem";
        case EntityIds::Item: return "minecraft:item";
        case EntityIds::LeashKnot: return "minecraft:leash_knot";
        case EntityIds::LightningBolt: return "minecraft:lightning_bolt";
        case EntityIds::LingeringPotion: return "minecraft:lingering_potion";
        case EntityIds::Llama: return "minecraft:llama";
        case EntityIds::LlamaSpit: return "minecraft:llama_spit";
        case EntityIds::MagmaCube: return "minecraft:magma_cube";
        case EntityIds::Minecart: return "minecraft:minecart";
        case EntityIds::Mooshroom: return "minecraft:mooshroom";
        case EntityIds::Mule: return "minecraft:mule";
        case EntityIds::Nautilus: return "minecraft:nautilus";
        case EntityIds::Npc: return "minecraft:npc";
        case EntityIds::Ocelot: return "minecraft:ocelot";
        case EntityIds::OminousItemSpawner: return "minecraft:ominous_item_spawner";
        case EntityIds::Painting: return "minecraft:painting";
        case EntityIds::Panda: return "minecraft:panda";
        case EntityIds::Parched: return "minecraft:parched";
        case EntityIds::Parrot: return "minecraft:parrot";
        case EntityIds::Phantom: return "minecraft:phantom";
        case EntityIds::Pig: return "minecraft:pig";
        case EntityIds::Piglin: return "minecraft:piglin";
        case EntityIds::PiglinBrute: return "minecraft:piglin_brute";
        case EntityIds::Pillager: return "minecraft:pillager";
        case EntityIds::Player: return "minecraft:player";
        case EntityIds::PolarBear: return "minecraft:polar_bear";
        case EntityIds::Pufferfish: return "minecraft:pufferfish";
        case EntityIds::Rabbit: return "minecraft:rabbit";
        case EntityIds::Ravager: return "minecraft:ravager";
        case EntityIds::Salmon: return "minecraft:salmon";
        case EntityIds::Sheep: return "minecraft:sheep";
        case EntityIds::Shulker: return "minecraft:shulker";
        case EntityIds::ShulkerBullet: return "minecraft:shulker_bullet";
        case EntityIds::Silverfish: return "minecraft:silverfish";
        case EntityIds::Skeleton: return "minecraft:skeleton";
        case EntityIds::SkeletonHorse: return "minecraft:skeleton_horse";
        case EntityIds::Slime: return "minecraft:slime";
        case EntityIds::SmallFireball: return "minecraft:small_fireball";
        case EntityIds::Sniffer: return "minecraft:sniffer";
        case EntityIds::SnowGolem: return "minecraft:snow_golem";
        case EntityIds::Snowball: return "minecraft:snowball";
        case EntityIds::Spider: return "minecraft:spider";
        case EntityIds::SplashPotion: return "minecraft:splash_potion";
        case EntityIds::Squid: return "minecraft:squid";
        case EntityIds::Stray: return "minecraft:stray";
        case EntityIds::Strider: return "minecraft:strider";
        case EntityIds::SulfurCube: return "minecraft:sulfur_cube";
        case EntityIds::Tadpole: return "minecraft:tadpole";
        case EntityIds::ThrownTrident: return "minecraft:thrown_trident";
        case EntityIds::Tnt: return "minecraft:tnt";
        case EntityIds::TntMinecart: return "minecraft:tnt_minecart";
        case EntityIds::TraderLlama: return "minecraft:trader_llama";
        case EntityIds::TripodCamera: return "minecraft:tripod_camera";
        case EntityIds::Tropicalfish: return "minecraft:tropicalfish";
        case EntityIds::Turtle: return "minecraft:turtle";
        case EntityIds::Vex: return "minecraft:vex";
        case EntityIds::Villager: return "minecraft:villager";
        case EntityIds::VillagerV2: return "minecraft:villager_v2";
        case EntityIds::Vindicator: return "minecraft:vindicator";
        case EntityIds::WanderingTrader: return "minecraft:wandering_trader";
        case EntityIds::Warden: return "minecraft:warden";
        case EntityIds::WindChargeProjectile: return "minecraft:wind_charge_projectile";
        case EntityIds::Witch: return "minecraft:witch";
        case EntityIds::Wither: return "minecraft:wither";
        case EntityIds::WitherSkeleton: return "minecraft:wither_skeleton";
        case EntityIds::WitherSkull: return "minecraft:wither_skull";
        case EntityIds::WitherSkullDangerous: return "minecraft:wither_skull_dangerous";
        case EntityIds::Wolf: return "minecraft:wolf";
        case EntityIds::XpBottle: return "minecraft:xp_bottle";
        case EntityIds::XpOrb: return "minecraft:xp_orb";
        case EntityIds::Zoglin: return "minecraft:zoglin";
        case EntityIds::Zombie: return "minecraft:zombie";
        case EntityIds::ZombieHorse: return "minecraft:zombie_horse";
        case EntityIds::ZombieNautilus: return "minecraft:zombie_nautilus";
        case EntityIds::ZombiePigman: return "minecraft:zombie_pigman";
        case EntityIds::ZombieVillager: return "minecraft:zombie_villager";
        case EntityIds::ZombieVillagerV2: return "minecraft:zombie_villager_v2";
        default: return invalid_tag;
    }
}

}
