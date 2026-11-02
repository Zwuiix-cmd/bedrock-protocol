#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class PrimitiveShapeType : uint8_t {
    Line = 0,
    Box = 1,
    Sphere = 2,
    Circle = 3,
    Text = 4,
    Arrow = 5,
    Cylinder = 6,
    Pyramid = 7,
    Ellipsoid = 8,
    Cone = 9,
};
