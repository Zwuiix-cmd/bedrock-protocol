#pragma once

#include <bitset>
#include <cstddef>
#include <cstdint>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

enum class InputData : int {
    ASCEND = 0,
    DESCEND = 1,
    NORTH_JUMP = 2,
    JUMP_DOWN = 3,
    SPRINT_DOWN = 4,
    CHANGE_HEIGHT = 5,
    JUMPING = 6,
    AUTO_JUMPING_IN_WATER = 7,
    SNEAKING = 8,
    SNEAK_DOWN = 9,
    UP = 10,
    DOWN = 11,
    LEFT = 12,
    RIGHT = 13,
    UP_LEFT = 14,
    UP_RIGHT = 15,
    WANT_UP = 16,
    WANT_DOWN = 17,
    WANT_DOWN_SLOW = 18,
    WANT_UP_SLOW = 19,
    SPRINTING = 20,
    ASCEND_BLOCK = 21,
    DESCEND_BLOCK = 22,
    SNEAK_TOGGLE_DOWN = 23,
    PERSIST_SNEAK = 24,
    START_SPRINTING = 25,
    STOP_SPRINTING = 26,
    START_SNEAKING = 27,
    STOP_SNEAKING = 28,
    START_SWIMMING = 29,
    STOP_SWIMMING = 30,
    START_JUMPING = 31,
    START_GLIDING = 32,
    STOP_GLIDING = 33,
    PERFORM_ITEM_INTERACTION = 34,
    PERFORM_BLOCK_ACTIONS = 35,
    PERFORM_ITEM_STACK_REQUEST = 36,
    HANDLED_TELEPORT = 37,
    EMOTING = 38,
    MISSED_SWING = 39,
    START_CRAWLING = 40,
    STOP_CRAWLING = 41,
    START_FLYING = 42,
    STOP_FLYING = 43,
    ACK_ACTOR_DATA = 44,
    IN_CLIENT_PREDICTED_VEHICLE = 45,
    PADDLING_LEFT = 46,
    PADDLING_RIGHT = 47,
    BLOCK_BREAKING_DELAY_ENABLED = 48,
    HORIZONTAL_COLLISION = 49,
    VERTICAL_COLLISION = 50,
    DOWN_LEFT = 51,
    DOWN_RIGHT = 52,
    START_USING_ITEM = 53,
    IS_CAMERA_RELATIVE_MOVEMENT_ENABLED = 54,
    IS_ROT_CONTROLLED_BY_MOVE_DIRECTION = 55,
    START_SPIN_ATTACK = 56,
    STOP_SPIN_ATTACK = 57,
    IS_HOTBAR_ONLY_TOUCH = 58,
    JUMP_RELEASED_RAW = 59,
    JUMP_PRESSED_RAW = 60,
    JUMP_CURRENT_RAW = 61,
    SNEAK_RELEASED_RAW = 62,
    SNEAK_PRESSED_RAW = 63,
    SNEAK_CURRENT_RAW = 64,
    INTERNAL_UPDATE = 65,
};

struct PlayerAuthInputFlags {
    static constexpr std::size_t Count =        66;

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