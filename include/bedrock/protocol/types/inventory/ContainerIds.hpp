#pragma once

#include <cstdint>
#include <string_view>

enum class ContainerIds : int32_t {
    None = -1,
    Inventory = 0,
    First = 1,
    Last = 100,
    Offhand = 119,
    Armor = 120,
    Hotbar = 122,
    FixedInventory = 123,
    Ui = 124,
    ContainerIdRegistry = 125,
};
