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
    Dolphin = 28,
    Donkey = 29,
    DragonFireball = 30,
    Drowned = 31,
    Egg = 32,
    ElderGuardian = 33,
    ElderGuardianGhost = 34,
    EnderCrystal = 35,
    EnderDragon = 36,
    EnderPearl = 37,
    Enderman = 38,
    Endermite = 39,
    EvocationFang = 40,
    EvocationIllager = 41,
    EyeOfEnderSignal = 42,
    FallingBlock = 43,
    Fireball = 44,
    FireworksRocket = 45,
    FishingHook = 46,
    Fox = 47,
    Frog = 48,
    Ghast = 49,
    GlowSquid = 50,
    Goat = 51,
    Guardian = 52,
    HappyGhast = 53,
    Hoglin = 54,
    HopperMinecart = 55,
    Horse = 56,
    Husk = 57,
    IceBomb = 58,
    IronGolem = 59,
    Item = 60,
    LeashKnot = 61,
    LightningBolt = 62,
    LingeringPotion = 63,
    Llama = 64,
    LlamaSpit = 65,
    MagmaCube = 66,
    Minecart = 67,
    Mooshroom = 68,
    Mule = 69,
    Nautilus = 70,
    Npc = 71,
    Ocelot = 72,
    OminousItemSpawner = 73,
    Painting = 74,
    Panda = 75,
    Parched = 76,
    Parrot = 77,
    Phantom = 78,
    Pig = 79,
    Piglin = 80,
    PiglinBrute = 81,
    Pillager = 82,
    Player = 83,
    PolarBear = 84,
    Pufferfish = 85,
    Rabbit = 86,
    Ravager = 87,
    Salmon = 88,
    Sheep = 89,
    Shulker = 90,
    ShulkerBullet = 91,
    Silverfish = 92,
    Skeleton = 93,
    SkeletonHorse = 94,
    Slime = 95,
    SmallFireball = 96,
    Sniffer = 97,
    SnowGolem = 98,
    Snowball = 99,
    Spider = 100,
    SplashPotion = 101,
    Squid = 102,
    Stray = 103,
    Strider = 104,
    Tadpole = 105,
    ThrownTrident = 106,
    Tnt = 107,
    TntMinecart = 108,
    TraderLlama = 109,
    TripodCamera = 110,
    Tropicalfish = 111,
    Turtle = 112,
    Vex = 113,
    Villager = 114,
    VillagerV2 = 115,
    Vindicator = 116,
    WanderingTrader = 117,
    Warden = 118,
    WindChargeProjectile = 119,
    Witch = 120,
    Wither = 121,
    WitherSkeleton = 122,
    WitherSkull = 123,
    WitherSkullDangerous = 124,
    Wolf = 125,
    XpBottle = 126,
    XpOrb = 127,
    Zoglin = 128,
    Zombie = 129,
    ZombieHorse = 130,
    ZombieNautilus = 131,
    ZombiePigman = 132,
    ZombieVillager = 133,
    ZombieVillagerV2 = 134,
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
        case EntityIds::Ghast: return "minecraft:ghast";
        case EntityIds::GlowSquid: return "minecraft:glow_squid";
        case EntityIds::Goat: return "minecraft:goat";
        case EntityIds::Guardian: return "minecraft:guardian";
        case EntityIds::HappyGhast: return "minecraft:happy_ghast";
        case EntityIds::Hoglin: return "minecraft:hoglin";
        case EntityIds::HopperMinecart: return "minecraft:hopper_minecart";
        case EntityIds::Horse: return "minecraft:horse";
        case EntityIds::Husk: return "minecraft:husk";
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
