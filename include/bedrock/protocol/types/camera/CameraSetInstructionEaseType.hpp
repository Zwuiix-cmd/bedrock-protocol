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

class CameraSetInstructionEaseType {
public:
    enum class In : uint8_t {
        Quad = 2,
        OutQuad = 4,
        Cubic = 5,
        OutCubic = 7,
        Quart = 8,
        OutQuart = 10,
        Quint = 11,
        OutQuint = 13,
        Sine = 14,
        OutSine = 16,
        Expo = 17,
        OutExpo = 19,
        Circ = 20,
        OutCirc = 22,
        Bounce = 23,
        OutBounce = 25,
        Back = 26,
        OutBack = 28,
        Elastic = 29,
        OutElastic = 31,
    };

    enum class Out : uint8_t {
        Quad = 3,
        Cubic = 6,
        Quart = 9,
        Quint = 12,
        Sine = 15,
        Expo = 18,
        Circ = 21,
        Bounce = 24,
        Back = 27,
        Elastic = 30,
    };

    static constexpr int64_t LINEAR = 0;
    static constexpr int64_t SPRING = 1;


    CameraSetInstructionEaseType() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        (void)stream;
    }

    virtual void write(BinaryStream& stream) const {
        (void)stream;
    }
};
