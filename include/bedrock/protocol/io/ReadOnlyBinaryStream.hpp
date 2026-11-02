#pragma once


#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <optional>
#include <vector>
#include <utility>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <bitset>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <type_traits>
#include <variant>

template <typename Enum>
requires std::is_enum_v<Enum>
[[nodiscard]] constexpr bool hasFlag(Enum value, Enum flag) noexcept {
    using Underlying = std::underlying_type_t<Enum>;
    return (static_cast<Underlying>(value) & static_cast<Underlying>(flag)) != 0;
}

class ReadOnlyBinaryStream {
public:
    template <typename T>
    T readValue() {
        T value;
        value.read(*this);
        return value;
    }

    template <typename Variant, typename Reader>
    Variant readOptionalVariant(Reader&& reader) {
        auto value = readOptional(std::forward<Reader>(reader));
        return value
            ? Variant{*value}
            : Variant{std::monostate{}};
    }

    template <typename Map, typename KeyReader, typename ValueReader>
    Map readMap(KeyReader&& keyReader, ValueReader&& valueReader) {
        Map result;
        const auto count = readUnsignedInt().value_or(0);
        for (std::size_t index = 0; index < count; ++index) {
            result.emplace(
                std::forward<KeyReader>(keyReader)(),
                std::forward<ValueReader>(valueReader)()
            );
        }
        return result;
    }

    // -- constructors --
    ReadOnlyBinaryStream() noexcept {
        setBuffer(nullptr, 0, false);
    }

    explicit ReadOnlyBinaryStream(std::string_view view) noexcept {
        setBuffer(reinterpret_cast<const uint8_t*>(view.data()), view.size(), false);
    }

    explicit ReadOnlyBinaryStream(std::string&& buffer) noexcept
            : ownedBuffer(std::move(buffer))
    {
        setBuffer(reinterpret_cast<const uint8_t*>(ownedBuffer.data()), ownedBuffer.size(), true);
    }

    explicit ReadOnlyBinaryStream(const std::string& buffer) noexcept {
        setBuffer(reinterpret_cast<const uint8_t*>(buffer.data()), buffer.size(), false);
    }

    ReadOnlyBinaryStream(const uint8_t* data, std::size_t size) noexcept {
        setBuffer(data, size, false);
    }

    virtual ~ReadOnlyBinaryStream() = default;

    // -- state --
    [[nodiscard]] bool hasOverflowed() const noexcept { return overflowed; }
    [[nodiscard]] std::size_t position() const noexcept { return readPointer; }
    [[nodiscard]] std::size_t size() const noexcept { return bufferSize; }
    [[nodiscard]] std::string_view data() const noexcept {
        return std::string_view(reinterpret_cast<const char*>(bufferData), bufferSize);
    }
    void seek(std::size_t pos, bool markOverflow = true) noexcept {
        readPointer = pos;
        overflowed = markOverflow || (readPointer > bufferSize);
    }

    // -- fast primitive read (trivially copyable) --
    template<typename T>
    inline std::optional<T> readPrimitive() noexcept {
        static_assert(std::is_trivially_copyable_v<T>, "readPrimitive requires trivially copyable type");
        const std::size_t len = sizeof(T);
        if (readPointer + len > bufferSize) {
            overflowed = true;
            return std::nullopt;
        }
        T out;
        std::memcpy(&out, bufferData + readPointer, len);
        readPointer += len;
        return out;
    }

    inline std::optional<int8_t>  readByte() noexcept { return readPrimitive<int8_t>(); }
    inline std::optional<int16_t> readShort() noexcept { return readPrimitive<int16_t>(); }
    inline std::optional<int32_t> readInt() noexcept { return readPrimitive<int32_t>(); }
    inline std::optional<int64_t> readLong() noexcept { return readPrimitive<int64_t>(); }
    inline std::optional<uint8_t> readUnsignedByte() noexcept { return readPrimitive<uint8_t>(); }
    inline std::optional<uint16_t> readUnsignedShort() noexcept { return readPrimitive<uint16_t>(); }
    inline std::optional<uint32_t> readUnsignedInt() noexcept { return readPrimitive<uint32_t>(); }
    inline std::optional<uint64_t> readUnsignedLong() noexcept { return readPrimitive<uint64_t>(); }
    inline std::optional<float> readFloat() noexcept { return readPrimitive<float>(); }
    inline std::optional<double> readDouble() noexcept { return readPrimitive<double>(); }

    inline std::optional<uint16_t> readUnsignedShortBE() noexcept {
        if (readPointer + 2 > bufferSize) {
            overflowed = true;
            return std::nullopt;
        }
        const uint16_t value = (static_cast<uint16_t>(bufferData[readPointer]) << 8)
            | static_cast<uint16_t>(bufferData[readPointer + 1]);
        readPointer += 2;
        return value;
    }

    inline std::optional<int16_t> readShortBE() noexcept {
        const auto value = readUnsignedShortBE();
        return value ? std::optional<int16_t>{static_cast<int16_t>(*value)} : std::nullopt;
    }

    inline std::optional<uint32_t> readUnsignedIntBE() noexcept {
        if (readPointer + 4 > bufferSize) {
            overflowed = true;
            return std::nullopt;
        }
        const uint32_t value = (static_cast<uint32_t>(bufferData[readPointer]) << 24)
            | (static_cast<uint32_t>(bufferData[readPointer + 1]) << 16)
            | (static_cast<uint32_t>(bufferData[readPointer + 2]) << 8)
            | static_cast<uint32_t>(bufferData[readPointer + 3]);
        readPointer += 4;
        return value;
    }

    inline std::optional<int32_t> readIntBE() noexcept {
        const auto value = readUnsignedIntBE();
        return value ? std::optional<int32_t>{static_cast<int32_t>(*value)} : std::nullopt;
    }

    inline std::optional<uint64_t> readUnsignedLongBE() noexcept {
        if (readPointer + 8 > bufferSize) {
            overflowed = true;
            return std::nullopt;
        }
        uint64_t value = 0;
        for (std::size_t index = 0; index < 8; ++index) {
            value = (value << 8) | static_cast<uint64_t>(bufferData[readPointer + index]);
        }
        readPointer += 8;
        return value;
    }

    inline std::optional<int64_t> readLongBE() noexcept {
        const auto value = readUnsignedLongBE();
        return value ? std::optional<int64_t>{static_cast<int64_t>(*value)} : std::nullopt;
    }

    inline std::optional<float> readFloatBE() noexcept {
        const auto value = readUnsignedIntBE();
        if (!value) return std::nullopt;
        float result = 0.0f;
        const uint32_t bits = *value;
        std::memcpy(&result, &bits, sizeof(result));
        return result;
    }

    inline std::optional<double> readDoubleBE() noexcept {
        const auto value = readUnsignedLongBE();
        if (!value) return std::nullopt;
        double result = 0.0;
        const uint64_t bits = *value;
        std::memcpy(&result, &bits, sizeof(result));
        return result;
    }

    inline std::optional<uint8_t> readByteFast() noexcept {
        if (readPointer >= bufferSize) { overflowed = true; return std::nullopt; }
        return bufferData[readPointer++];
    }

    inline bool readBytes(void* out, std::size_t len) noexcept {
        if (len == 0) return true;
        if (readPointer + len > bufferSize) { overflowed = true; return false; }
        std::memcpy(out, bufferData + readPointer, len);
        readPointer += len;
        return true;
    }

    inline std::optional<std::string> readUUID() noexcept {
        if (readPointer + 16 > bufferSize) {
            overflowed = true;
            return std::nullopt;
        }

        std::array<uint8_t, 16> buffer{};
        std::memcpy(buffer.data(), bufferData + readPointer, 16);
        readPointer += 16;

        for (int i = 0; i < 4; ++i) {
            std::swap(buffer[i], buffer[7 - i]);
        }
        for (int i = 0; i < 4; ++i) {
            std::swap(buffer[8 + i], buffer[15 - i]);
        }

        return uuidToString(buffer);
    }

    inline std::optional<std::string_view> readStringView() noexcept {
        auto sizeOpt = readUnsignedVarInt();
        if (!sizeOpt) return std::nullopt;
        const uint32_t len = *sizeOpt;
        if (len == 0) return std::string_view{};
        if (readPointer + len > bufferSize) { overflowed = true; return std::nullopt; }
        std::string_view view(reinterpret_cast<const char*>(bufferData + readPointer), len);
        readPointer += len;
        return view;
    }

    inline bool readString(std::string& out) noexcept {
        auto sizeOpt = readUnsignedVarInt();
        if (!sizeOpt) return false;
        const uint32_t len = *sizeOpt;

        if (static_cast<std::size_t>(len) > bufferSize - readPointer) {
            overflowed = true;
            return false;
        }

        if (len == 0) {
            out.clear();
            return true;
        }

        out.assign(reinterpret_cast<const char*>(bufferData + readPointer), len);
        readPointer += len;
        return true;
    }

    inline std::optional<std::string> readString() noexcept {
        std::string out;
        if (!readString(out)) return std::nullopt;
        return out;
    }

    inline std::optional<std::string> readStringNBT() noexcept {
        auto sizeOpt = readUnsignedShort();
        if (!sizeOpt) return std::nullopt;
        const uint16_t len = *sizeOpt;
        if (readPointer + len > bufferSize) { overflowed = true; return std::nullopt; }
        std::string s(reinterpret_cast<const char*>(bufferData + readPointer), len);
        readPointer += len;
        return s;
    }

    inline std::optional<std::vector<uint8_t>> readByteArray() noexcept {
        auto sizeOpt = readInt();
        if (!sizeOpt) return std::nullopt;
        int32_t len = *sizeOpt;
        if (len < 0) { overflowed = true; return std::nullopt; }
        if (readPointer + static_cast<std::size_t>(len) > bufferSize) { overflowed = true; return std::nullopt; }
        std::vector<uint8_t> vec;
        vec.resize(len);
        std::memcpy(vec.data(), bufferData + readPointer, len);
        readPointer += len;
        return vec;
    }

    inline std::optional<std::string> readByteArray(std::size_t len) noexcept {
        if (len > bufferSize - readPointer) {
            overflowed = true;
            return std::nullopt;
        }
        std::string value(reinterpret_cast<const char*>(bufferData + readPointer), len);
        readPointer += len;
        return value;
    }

    inline std::optional<uint32_t> readUnsignedVarInt() noexcept {
        uint32_t value = 0;
        int shift = 0;
        while (true) {
            if (readPointer >= bufferSize) { overflowed = true; return std::nullopt; }
            uint8_t byte = bufferData[readPointer++];
            value |= static_cast<uint32_t>(byte & 0x7Fu) << shift;
            if ((byte & 0x80u) == 0) return value;
            shift += 7;
            if (shift >= 32) { overflowed = true; return std::nullopt; }
        }
    }

    inline std::optional<uint64_t> readUnsignedVarLong() noexcept {
        uint64_t value = 0;
        int shift = 0;
        while (true) {
            if (readPointer >= bufferSize) { overflowed = true; return std::nullopt; }
            uint8_t byte = bufferData[readPointer++];
            value |= static_cast<uint64_t>(byte & 0x7Fu) << shift;
            if ((byte & 0x80u) == 0) return value;
            shift += 7;
            if (shift >= 64) { overflowed = true; return std::nullopt; }
        }
    }

    inline std::optional<int32_t> readVarInt() noexcept {
        auto u = readUnsignedVarInt();
        if (!u) return std::nullopt;
        return decodeZigZag32(*u);
    }

    inline std::optional<int64_t> readVarLong() noexcept {
        auto u = readUnsignedVarLong();
        if (!u) return std::nullopt;
        return decodeZigZag64(*u);
    }

    inline int32_t decodeZigZag32(int64_t value) noexcept {
        return static_cast<int32_t>((static_cast<uint64_t>(value) >> 1) ^ -static_cast<int32_t>(value & 1));
    }

    inline int64_t decodeZigZag64(int64_t value) noexcept {
        return (value >> 1) ^ -(value & 1);
    }

    template <std::size_t N>
    inline std::optional<std::bitset<N>> decodeBitset() noexcept {
        std::bitset<N> result;
        std::size_t shift = 0;
        while (shift < N) {
            if (readPointer >= bufferSize) { overflowed = true; return std::nullopt; }
            uint8_t byte = bufferData[readPointer++];
            uint8_t value = byte & 0x7Fu;
            for (int i = 0; i < 7 && shift < N; ++i, ++shift) {
                if (value & (1u << i)) result.set(shift);
            }
            if ((byte & 0x80u) == 0) {
                return result;
            }
        }
        return std::nullopt;
    }

    inline std::optional<glm::ivec3> readBlockPos() noexcept {
        auto x = readVarInt();
        auto y = readUnsignedVarInt();
        auto z = readVarInt();
        if (!x || !y || !z) return std::nullopt;
        return glm::ivec3{ *x, static_cast<int>(*y), *z };
    }

    inline std::optional<glm::ivec3> readSignedBlockPos() noexcept {
        auto x = readVarInt();
        auto y = readVarInt();
        auto z = readVarInt();
        if (!x || !y || !z) return std::nullopt;
        return glm::ivec3{ *x, static_cast<int>(*y), *z };
    }

    inline std::optional<int64_t> readActorUniqueID() noexcept {
        auto v = readVarLong();
        if (!v) return std::nullopt;
        return int64_t{ *v };
    }

    inline std::optional<uint64_t> readActorRuntimeID() noexcept {
        auto v = readUnsignedVarLong();
        if (!v) return std::nullopt;
        return uint64_t{ *v };
    }

    inline std::optional<glm::vec3> readVec3() noexcept {
        auto x = readFloat();
        auto y = readFloat();
        auto z = readFloat();
        if (!x && !y && !z) return std::nullopt;
        return glm::vec3{ x.value_or(0.0f), y.value_or(0.0f), z.value_or(0.0f) };
    }

    inline std::optional<glm::vec2> readVec2() noexcept {
        auto x = readFloat();
        auto y = readFloat();
        if (!x && !y) return std::nullopt;
        return glm::vec2{ x.value_or(0.0f), y.value_or(0.0f) };
    }

    inline std::optional<glm::vec2> readRotationVec2() noexcept {
        auto x = readRotationByte();
        auto y = readRotationByte();
        if (!x && !y) return std::nullopt;
        return glm::vec2{ x.value_or(0.0f), y.value_or(0.0f) };
    }

    inline std::optional<float> readRotationByte() noexcept {
        auto b = readUnsignedByte();
        if (!b) return std::nullopt;
        return static_cast<float>(*b) * (360.0f / 256.0f);
    }

    inline std::optional<bool> readBool() noexcept {
        auto b = readByteFast();
        if (!b) return std::nullopt;
        return *b != 0;
    }

    template <typename Reader>
    auto readOptional(Reader&& reader) {
        using Value = std::remove_cvref_t<std::invoke_result_t<Reader&>>;
        if (!readBool().value_or(false)) return std::optional<Value>{};
        return std::optional<Value>{std::forward<Reader>(reader)()};
    }

    template <typename Reader>
    auto readOptionalWhenFalse(Reader&& reader) {
        using Value = std::remove_cvref_t<std::invoke_result_t<Reader&>>;
        if (readBool().value_or(false)) return std::optional<Value>{};
        return std::optional<Value>{std::forward<Reader>(reader)()};
    }

    template <typename Reader>
    auto readList(Reader&& reader) {
        using Value = std::remove_cvref_t<std::invoke_result_t<Reader&>>;
        const uint32_t count = readUnsignedVarInt().value_or(0);
        std::vector<Value> values;
        values.reserve(count);
        for (uint32_t index = 0; index < count; ++index) {
            values.emplace_back(std::forward<Reader>(reader)());
        }
        return values;
    }

    template <typename T>
    std::shared_ptr<T> readObject() {
        auto value = std::make_shared<T>();
        value->read(*this);
        return value;
    }

    template <typename T>
    std::vector<std::shared_ptr<T>> readObjectList() {
        const auto count = readUnsignedVarInt().value_or(0);
        std::vector<std::shared_ptr<T>> values;
        values.reserve(count);
        for (uint32_t index = 0; index < count; ++index) {
            auto value = std::make_shared<T>();
            value->read(*this);
            values.emplace_back(std::move(value));
        }
        return values;
    }

protected:
    // allow derived classes (BinaryStream) to refresh pointer/size after mutation
    inline void refreshFromOwned() noexcept {
        bufferData = reinterpret_cast<const uint8_t*>(ownedBuffer.data());
        bufferSize = ownedBuffer.size();
        if (readPointer > bufferSize) {
            overflowed = true;
            readPointer = bufferSize;
        }
    }

    // internal state (protected so derived mutators can access owned storage if needed)
    std::string ownedBuffer;            // only populated when we own memory
    const uint8_t* bufferData = nullptr;      // pointer to current buffer memory
    std::size_t bufferSize = 0;               // length of buffer
    std::size_t readPointer = 0;        // current read pos (offset from bufferData)
    bool overflowed = false;

private:
    inline void setBuffer(const uint8_t* data, std::size_t size, bool owned) noexcept {
        bufferData = data;
        bufferSize = size;
        readPointer = 0;
        overflowed = false;
        // ownedBuffer is already set by ctor if owned
    }

    static inline std::string uuidToString(const std::array<uint8_t, 16>& uuid) {
        constexpr char hex[] = "0123456789abcdef";
        std::string result(36, '\0');
        std::size_t outputIndex = 0;

        for (std::size_t i = 0; i < uuid.size(); ++i) {
            if (i == 4 || i == 6 || i == 8 || i == 10) {
                result[outputIndex++] = '-';
            }
            result[outputIndex++] = hex[uuid[i] >> 4];
            result[outputIndex++] = hex[uuid[i] & 0x0F];
        }

        return result;
    }
};

