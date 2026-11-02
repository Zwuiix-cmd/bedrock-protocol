#pragma once

#include <bitset>
#include <cstddef>
#include <cstdint>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

namespace bedrock::protocol {


enum class InputData : int {
    Ascend = 0,
    Descend = 1,
    NorthJump = 2,
    JumpDown = 3,
    SprintDown = 4,
    ChangeHeight = 5,
    Jumping = 6,
    AutoJumpingInWater = 7,
    Sneaking = 8,
    SneakDown = 9,
    Up = 10,
    Down = 11,
    Left = 12,
    Right = 13,
    UpLeft = 14,
    UpRight = 15,
    WantUp = 16,
    WantDown = 17,
    WantDownSlow = 18,
    WantUpSlow = 19,
    Sprinting = 20,
    AscendBlock = 21,
    DescendBlock = 22,
    SneakToggleDown = 23,
    PersistSneak = 24,
    StartSprinting = 25,
    StopSprinting = 26,
    StartSneaking = 27,
    StopSneaking = 28,
    StartSwimming = 29,
    StopSwimming = 30,
    StartJumping = 31,
    StartGliding = 32,
    StopGliding = 33,
    PerformItemInteraction = 34,
    PerformBlockActions = 35,
    PerformItemStackRequest = 36,
    HandledTeleport = 37,
    Emoting = 38,
    MissedSwing = 39,
    StartCrawling = 40,
    StopCrawling = 41,
    StartFlying = 42,
    StopFlying = 43,
    AckActorData = 44,
    InClientPredictedVehicle = 45,
    PaddlingLeft = 46,
    PaddlingRight = 47,
    BlockBreakingDelayEnabled = 48,
    HorizontalCollision = 49,
    VerticalCollision = 50,
    DownLeft = 51,
    DownRight = 52,
    StartUsingItem = 53,
    IsCameraRelativeMovementEnabled = 54,
    IsRotControlledByMoveDirection = 55,
    StartSpinAttack = 56,
    StopSpinAttack = 57,
    IsHotbarOnlyTouch = 58,
    JumpReleasedRaw = 59,
    JumpPressedRaw = 60,
    JumpCurrentRaw = 61,
    SneakReleasedRaw = 62,
    SneakPressedRaw = 63,
    SneakCurrentRaw = 64,
};

struct PlayerAuthInputFlags {
    static constexpr std::size_t Count =        65;

    std::bitset<Count> flags;

    PlayerAuthInputFlags() = default;
    explicit PlayerAuthInputFlags(std::size_t) {}

    [[nodiscard]] bool hasFlag(InputData flag) const noexcept {
        return flags.test(static_cast<std::size_t>(flag));
    }

    [[nodiscard]] bool get(std::size_t flag) const noexcept {
        return flag < Count && flags.test(flag);
    }

    [[nodiscard]] bool get(InputData flag) const noexcept {
        return get(static_cast<std::size_t>(flag));
    }

    [[nodiscard]] std::size_t getLength() const noexcept {
        return Count;
    }

    void setFlag(InputData flag, bool state) noexcept {
        flags.set(static_cast<std::size_t>(flag), state);
    }

    void set(std::size_t flag, bool state) noexcept {
        if (flag < Count) {
            flags.set(flag, state);
        }
    }

    void addFlag(InputData flag) noexcept {
        setFlag(flag, true);
    }

    void removeFlag(InputData flag) noexcept {
        setFlag(flag, false);
    }

    void write(BinaryStream& stream) const {
        for (std::size_t offset = 0; offset < Count; offset += 7) {
            uint8_t byte = 0;
            for (std::size_t bit = 0; bit < 7 && offset + bit < Count; ++bit) {
                if (flags.test(offset + bit)) {
                    byte |= static_cast<uint8_t>(uint8_t{1} << bit);
                }
            }
            if (offset + 7 < Count) {
                byte |= uint8_t{0x80};
            }
            stream.writeByte(static_cast<int8_t>(byte));
        }
    }

    void read(ReadOnlyBinaryStream& stream) {
        flags.reset();
        for (std::size_t offset = 0; offset < Count; offset += 7) {
            const uint8_t byte = static_cast<uint8_t>(stream.readByte().value_or(0));
            const uint8_t payload = static_cast<uint8_t>(byte & uint8_t{0x7f});
            for (std::size_t bit = 0; bit < 7 && offset + bit < Count; ++bit) {
                if ((payload & static_cast<uint8_t>(uint8_t{1} << bit)) != 0) {
                    flags.set(offset + bit);
                }
            }
            if ((byte & uint8_t{0x80}) == 0) {
                break;
            }
        }
    }
};

} // namespace bedrock::protocol
