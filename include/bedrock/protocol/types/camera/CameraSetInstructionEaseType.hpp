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
    static constexpr int64_t LINEAR = 0;
    static constexpr int64_t SPRING = 1;
    static constexpr int64_t IN_QUAD = 2;
    static constexpr int64_t OUT_QUAD = 3;
    static constexpr int64_t IN_OUT_QUAD = 4;
    static constexpr int64_t IN_CUBIC = 5;
    static constexpr int64_t OUT_CUBIC = 6;
    static constexpr int64_t IN_OUT_CUBIC = 7;
    static constexpr int64_t IN_QUART = 8;
    static constexpr int64_t OUT_QUART = 9;
    static constexpr int64_t IN_OUT_QUART = 10;
    static constexpr int64_t IN_QUINT = 11;
    static constexpr int64_t OUT_QUINT = 12;
    static constexpr int64_t IN_OUT_QUINT = 13;
    static constexpr int64_t IN_SINE = 14;
    static constexpr int64_t OUT_SINE = 15;
    static constexpr int64_t IN_OUT_SINE = 16;
    static constexpr int64_t IN_EXPO = 17;
    static constexpr int64_t OUT_EXPO = 18;
    static constexpr int64_t IN_OUT_EXPO = 19;
    static constexpr int64_t IN_CIRC = 20;
    static constexpr int64_t OUT_CIRC = 21;
    static constexpr int64_t IN_OUT_CIRC = 22;
    static constexpr int64_t IN_BOUNCE = 23;
    static constexpr int64_t OUT_BOUNCE = 24;
    static constexpr int64_t IN_OUT_BOUNCE = 25;
    static constexpr int64_t IN_BACK = 26;
    static constexpr int64_t OUT_BACK = 27;
    static constexpr int64_t IN_OUT_BACK = 28;
    static constexpr int64_t IN_ELASTIC = 29;
    static constexpr int64_t OUT_ELASTIC = 30;
    static constexpr int64_t IN_OUT_ELASTIC = 31;


    CameraSetInstructionEaseType() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        (void)stream;
    }

    virtual void write(BinaryStream& stream) const {
        (void)stream;
    }
};
