#pragma once


#include "ReadOnlyBinaryStream.hpp"
#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <utility>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <type_traits>

namespace bedrock::protocol {

class BinaryStream : public ReadOnlyBinaryStream {
public:
    BinaryStream() noexcept : ReadOnlyBinaryStream() {
        // ensure owned buffer pointer points to empty owned buffer
        ownedBuffer.clear();
        refreshFromOwned();
    }

    explicit BinaryStream(std::string&& buffer) noexcept : ReadOnlyBinaryStream(std::move(buffer)) {
        // ReadOnlyBinaryStream ctor already moved the string into ownedBuffer and set the pointer,
        // but ensure consistency
        refreshFromOwned();
    }

    // construct non-owning view - but BinaryStream must own the data to write into it.
    // if user passes const std::string&, we make an owned copy.
    explicit BinaryStream(const std::string& buffer) noexcept {
        ownedBuffer = buffer; // copy
        refreshFromOwned();
    }

    // reset buffer to empty (clear and refresh internal pointers)
    inline void reset() noexcept {
        ownedBuffer.clear();
        refreshFromOwned();
    }

    // low-level write: append raw bytes to the owned buffer
    inline void write(const void* data, std::size_t len) {
        if (len == 0) return;
        // append raw bytes
        ownedBuffer.append(reinterpret_cast<const char*>(data), len);
        // update internal pointer/size after append (bufferData may have changed due to reallocation)
        refreshFromOwned();
    }

    // convenience write for whole string
    inline void writeBuffer(const std::string& buffer) {
        write(buffer.data(), buffer.size());
    }

    // completely overwrite owned buffer
    inline void overwriteBuffer(const std::string& buffer) {
        ownedBuffer = buffer;
        refreshFromOwned();
    }

    // primitive writes (trivially copyable)
    template <typename prim_t>
    inline void writePrimitive(const prim_t& value) {
        static_assert(std::is_trivially_copyable_v<prim_t>, "writePrimitive requires trivially copyable type");
        write(&value, sizeof(prim_t));
    }

    inline void writeBool(bool value) { writePrimitive<bool>(value); }

    template <typename T, typename Writer>
    void writeOptional(const std::optional<T>& value, Writer&& writer) {
        writeBool(value.has_value());
        if (value.has_value()) std::forward<Writer>(writer)(*value);
    }

    template <typename T, typename Writer>
    void writeOptionalWhenFalse(const std::optional<T>& value, Writer&& writer) {
        writeBool(!value.has_value());
        if (value.has_value()) std::forward<Writer>(writer)(*value);
    }

    template <typename T, typename Writer>
    void writeList(const std::vector<T>& values, Writer&& writer) {
        writeUnsignedVarInt(static_cast<uint32_t>(values.size()));
        for (const auto& value : values) {
            std::forward<Writer>(writer)(value);
        }
    }

    inline void writeByte(int8_t value) { writePrimitive<int8_t>(value); }
    inline void writeShort(int16_t value) { writePrimitive<int16_t>(value); }
    inline void writeInt(int32_t value) { writePrimitive<int32_t>(value); }
    inline void writeLong(int64_t value) { writePrimitive<int64_t>(value); }

    inline void writeUnsignedByte(uint8_t value) { writePrimitive<uint8_t>(value); }
    inline void writeUnsignedShort(uint16_t value) { writePrimitive<uint16_t>(value); }
    inline void writeUnsignedInt(uint32_t value) { writePrimitive<uint32_t>(value); }
    inline void writeUnsignedLong(uint64_t value) { writePrimitive<uint64_t>(value); }

    template <typename Enum>
    requires std::is_enum_v<Enum>
    inline void writeByte(Enum value) {
        writeByte(static_cast<int8_t>(value));
    }

    template <typename Enum>
    requires std::is_enum_v<Enum>
    inline void writeShort(Enum value) {
        writeShort(static_cast<int16_t>(value));
    }

    template <typename Enum>
    requires std::is_enum_v<Enum>
    inline void writeInt(Enum value) {
        writeInt(static_cast<int32_t>(value));
    }

    template <typename Enum>
    requires std::is_enum_v<Enum>
    inline void writeLong(Enum value) {
        writeLong(static_cast<int64_t>(value));
    }

    template <typename Enum>
    requires std::is_enum_v<Enum>
    inline void writeUnsignedByte(Enum value) {
        writeUnsignedByte(static_cast<uint8_t>(value));
    }

    template <typename Enum>
    requires std::is_enum_v<Enum>
    inline void writeUnsignedShort(Enum value) {
        writeUnsignedShort(static_cast<uint16_t>(value));
    }

    template <typename Enum>
    requires std::is_enum_v<Enum>
    inline void writeUnsignedInt(Enum value) {
        writeUnsignedInt(static_cast<uint32_t>(value));
    }

    template <typename Enum>
    requires std::is_enum_v<Enum>
    inline void writeUnsignedLong(Enum value) {
        writeUnsignedLong(static_cast<uint64_t>(value));
    }

    inline void writeShortBE(int16_t value) { writeUnsignedShortBE(static_cast<uint16_t>(value)); }

    inline void writeUnsignedShortBE(uint16_t value) {
        const std::array<uint8_t, 2> bytes{
            static_cast<uint8_t>(value >> 8),
            static_cast<uint8_t>(value),
        };
        write(bytes.data(), bytes.size());
    }

    inline void writeIntBE(int32_t value) {
        writeUnsignedIntBE(static_cast<uint32_t>(value));
    }

    inline void writeUnsignedIntBE(uint32_t value) {
        const std::array<uint8_t, 4> bytes{
            static_cast<uint8_t>(value >> 24),
            static_cast<uint8_t>(value >> 16),
            static_cast<uint8_t>(value >> 8),
            static_cast<uint8_t>(value),
        };
        write(bytes.data(), bytes.size());
    }

    inline void writeLongBE(int64_t value) { writeUnsignedLongBE(static_cast<uint64_t>(value)); }

    inline void writeUnsignedLongBE(uint64_t value) {
        const std::array<uint8_t, 8> bytes{
            static_cast<uint8_t>(value >> 56),
            static_cast<uint8_t>(value >> 48),
            static_cast<uint8_t>(value >> 40),
            static_cast<uint8_t>(value >> 32),
            static_cast<uint8_t>(value >> 24),
            static_cast<uint8_t>(value >> 16),
            static_cast<uint8_t>(value >> 8),
            static_cast<uint8_t>(value),
        };
        write(bytes.data(), bytes.size());
    }

    inline void writeFloatBE(float value) {
        uint32_t bits = 0;
        std::memcpy(&bits, &value, sizeof(bits));
        writeUnsignedIntBE(bits);
    }

    inline void writeDoubleBE(double value) {
        uint64_t bits = 0;
        std::memcpy(&bits, &value, sizeof(bits));
        writeUnsignedLongBE(bits);
    }

    // Varint helpers - using local buffer append for speed (no intermediate allocations)
    inline void writeVarInt(int32_t value) {
        // ZigZag encode
        uint32_t uv = static_cast<uint32_t>((value << 1) ^ (value >> 31));
        writeUnsignedVarInt(uv);
    }

    template <typename Enum>
    requires std::is_enum_v<Enum>
    inline void writeVarInt(Enum value) {
        writeVarInt(static_cast<int32_t>(value));
    }

    inline void writeUnsignedVarInt(uint32_t value) {
        std::array<char, 5> bytes{};
        std::size_t count = 0;
        do {
            uint8_t byte = static_cast<uint8_t>(value & 0x7Fu);
            value >>= 7;
            if (value != 0u) byte |= 0x80u;
            bytes[count++] = static_cast<char>(byte);
        } while (value != 0u);
        ownedBuffer.append(bytes.data(), count);
        refreshFromOwned();
    }

    inline void writeVarLong(int64_t value) {
        uint64_t uv = static_cast<uint64_t>((value << 1) ^ (value >> 63));
        writeUnsignedVarLong(uv);
    }

    inline void writeUnsignedVarLong(uint64_t value) {
        std::array<char, 10> bytes{};
        std::size_t count = 0;
        do {
            uint8_t byte = static_cast<uint8_t>(value & 0x7Fu);
            value >>= 7;
            if (value != 0ull) byte |= 0x80u;
            bytes[count++] = static_cast<char>(byte);
        } while (value != 0ull);
        ownedBuffer.append(bytes.data(), count);
        refreshFromOwned();
    }

    template <typename Enum>
    requires std::is_enum_v<Enum>
    inline void writeUnsignedVarInt(Enum value) {
        writeUnsignedVarInt(static_cast<uint32_t>(value));
    }

    template <typename Enum>
    requires std::is_enum_v<Enum>
    inline void writeVarLong(Enum value) {
        writeVarLong(static_cast<int64_t>(value));
    }

    template <typename Enum>
    requires std::is_enum_v<Enum>
    inline void writeUnsignedVarLong(Enum value) {
        writeUnsignedVarLong(static_cast<uint64_t>(value));
    }

    inline void writeFloat(float value) { writePrimitive<float>(value); }
    inline void writeDouble(double value) { writePrimitive<double>(value); }

    inline void writeFixedFloat(float value, double d) {
        writeVarInt(static_cast<int32_t>(value * d));
    }

    inline void writeNormalizedFloat(float value) {
        // 2^31 - 1 as double is 2147483647.0
        writeFixedFloat(value, 2147483647.0);
    }

    inline void writeUUID(std::string_view uuidStr) {
        auto uuidBin = stringToUUID(uuidStr);

        for (int i = 0; i < 4; ++i) {
            std::swap(uuidBin[i], uuidBin[7 - i]);
        }
        for (int i = 0; i < 4; ++i) {
            std::swap(uuidBin[8 + i], uuidBin[15 - i]);
        }

        write(uuidBin.data(), 16);
    }

    inline void writeString(std::string_view buffer) {
        uint32_t length = static_cast<uint32_t>(buffer.size());
        std::array<char, 5> prefix{};
        std::size_t prefixSize = 0;
        do {
            uint8_t byte = static_cast<uint8_t>(length & 0x7Fu);
            length >>= 7;
            if (length != 0u) byte |= 0x80u;
            prefix[prefixSize++] = static_cast<char>(byte);
        } while (length != 0u);

        ownedBuffer.reserve(ownedBuffer.size() + prefixSize + buffer.size());
        ownedBuffer.append(prefix.data(), prefixSize);
        if (!buffer.empty()) ownedBuffer.append(buffer.data(), buffer.size());
        refreshFromOwned();
    }

    inline void writeStringNBT(const std::string& buffer) {
        writeUnsignedShort(static_cast<uint16_t>(buffer.size()));
        if (!buffer.empty()) write(buffer.data(), buffer.size());
    }

    inline void writeByteArray(const std::vector<uint8_t>& buffer) {
        writeInt(static_cast<int32_t>(buffer.size()));
        if (!buffer.empty()) write(buffer.data(), buffer.size());
    }

    inline void writeByteArray(std::string_view buffer) {
        if (!buffer.empty()) write(buffer.data(), buffer.size());
    }

    inline void writeIntArray(const std::vector<int>& buffer) {
        writeUnsignedInt(static_cast<uint32_t>(buffer.size()));
        for (int val : buffer) {
            writeInt(val);
        }
    }

    inline void writeStringNT(const std::string& buffer) {
        if (!buffer.empty()) write(buffer.data(), buffer.size());
        writeUnsignedByte(0);
    }

    inline void writeBlockPos(const glm::ivec3& pos) {
        writeVarInt(pos.x);
        writeUnsignedVarInt(static_cast<uint32_t>(pos.y));
        writeVarInt(pos.z);
    }

    inline void writeSignedBlockPos(const glm::ivec3& pos) {
        writeVarInt(pos.x);
        writeVarInt(pos.y);
        writeVarInt(pos.z);
    }

    inline void writeActorUniqueID(const int64_t& actor) {
        writeVarLong(actor);
    }

    inline void writeActorRuntimeID(const uint64_t& actor) {
        writeUnsignedVarLong(actor);
    }

    inline void writeEmptyItem() {
        writeVarInt(0);
    }

    inline void writeVec3(const glm::vec3& vec) {
        writeFloat(vec.x);
        writeFloat(vec.y);
        writeFloat(vec.z);
    }

    inline void writeVec2(const glm::vec2& vec) {
        writeFloat(vec.x);
        writeFloat(vec.y);
    }

    inline void writeRotationVec2(const glm::vec2& vec) {
        writeRotationByte(vec.x);
        writeRotationByte(vec.y);
    }

    inline void writeRotationByte(float rotation) {
        auto angle = static_cast<uint8_t>(rotation / (360.0f / 256.0f));
        writeUnsignedByte(angle);
    }

    inline const std::string& getBuffer() const noexcept {
        return ownedBuffer;
    }

    inline std::string data() const {
        return ownedBuffer;
    }

    static constexpr uint32_t flipIntEndianness(uint32_t value) noexcept {
        return ((value & 0x000000FFu) << 24)
            | ((value & 0x0000FF00u) << 8)
            | ((value & 0x00FF0000u) >> 8)
            | ((value & 0xFF000000u) >> 24);
    }

    // If you need the ReadOnlyBinaryStream view to reflect new owned buffer,
    // call refreshFromOwned() (BinaryStream already calls it after writes).
private:
    static inline uint8_t hexValue(char value) noexcept {
        if (value >= '0' && value <= '9') return static_cast<uint8_t>(value - '0');
        if (value >= 'a' && value <= 'f') return static_cast<uint8_t>(value - 'a' + 10);
        if (value >= 'A' && value <= 'F') return static_cast<uint8_t>(value - 'A' + 10);
        return 0xFF;
    }

    static inline std::array<uint8_t, 16> stringToUUID(std::string_view str) {
        std::array<uint8_t, 16> uuid{};
        std::size_t byteIndex = 0;
        uint8_t highNibble = 0;
        bool hasHighNibble = false;

        for (const char value : str) {
            if (value == '-') continue;

            const uint8_t nibble = hexValue(value);
            if (nibble == 0xFF || byteIndex >= uuid.size()) {
                return {};
            }

            if (!hasHighNibble) {
                highNibble = nibble;
                hasHighNibble = true;
            } else {
                uuid[byteIndex++] = static_cast<uint8_t>((highNibble << 4) | nibble);
                hasHighNibble = false;
            }
        }

        if (byteIndex != uuid.size() || hasHighNibble) return {};
        return uuid;
    }
};

} // namespace bedrock::protocol

