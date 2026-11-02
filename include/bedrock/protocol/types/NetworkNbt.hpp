#pragma once


#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace bedrock::protocol {

enum class NbtTagType : uint8_t {
    End = 0,
    Byte = 1,
    Short = 2,
    Int = 3,
    Long = 4,
    Float = 5,
    Double = 6,
    ByteArray = 7,
    String = 8,
    List = 9,
    Compound = 10,
    IntArray = 11,
    LongArray = 12,
};

class NbtTag {
public:
    virtual ~NbtTag() = default;
    [[nodiscard]] virtual NbtTagType getType() const noexcept = 0;
    [[nodiscard]] virtual std::unique_ptr<NbtTag> clone() const = 0;
};

class ByteTag : public NbtTag {
public:
    int8_t value = 0;

    ByteTag() = default;
    explicit ByteTag(int8_t value) : value(value) {}
    [[nodiscard]] NbtTagType getType() const noexcept override { return NbtTagType::Byte; }
    [[nodiscard]] std::unique_ptr<NbtTag> clone() const override { return std::make_unique<ByteTag>(*this); }
};

class ShortTag : public NbtTag {
public:
    int16_t value = 0;

    ShortTag() = default;
    explicit ShortTag(int16_t value) : value(value) {}
    [[nodiscard]] NbtTagType getType() const noexcept override { return NbtTagType::Short; }
    [[nodiscard]] std::unique_ptr<NbtTag> clone() const override { return std::make_unique<ShortTag>(*this); }
};

class IntTag : public NbtTag {
public:
    int32_t value = 0;

    IntTag() = default;
    explicit IntTag(int32_t value) : value(value) {}
    [[nodiscard]] NbtTagType getType() const noexcept override { return NbtTagType::Int; }
    [[nodiscard]] std::unique_ptr<NbtTag> clone() const override { return std::make_unique<IntTag>(*this); }
};

class LongTag : public NbtTag {
public:
    int64_t value = 0;

    LongTag() = default;
    explicit LongTag(int64_t value) : value(value) {}
    [[nodiscard]] NbtTagType getType() const noexcept override { return NbtTagType::Long; }
    [[nodiscard]] std::unique_ptr<NbtTag> clone() const override { return std::make_unique<LongTag>(*this); }
};

class FloatTag : public NbtTag {
public:
    float value = 0.0f;

    FloatTag() = default;
    explicit FloatTag(float value) : value(value) {}
    [[nodiscard]] NbtTagType getType() const noexcept override { return NbtTagType::Float; }
    [[nodiscard]] std::unique_ptr<NbtTag> clone() const override { return std::make_unique<FloatTag>(*this); }
};

class DoubleTag : public NbtTag {
public:
    double value = 0.0;

    DoubleTag() = default;
    explicit DoubleTag(double value) : value(value) {}
    [[nodiscard]] NbtTagType getType() const noexcept override { return NbtTagType::Double; }
    [[nodiscard]] std::unique_ptr<NbtTag> clone() const override { return std::make_unique<DoubleTag>(*this); }
};

class ByteArrayTag : public NbtTag {
public:
    std::vector<int8_t> value;
    [[nodiscard]] NbtTagType getType() const noexcept override { return NbtTagType::ByteArray; }
    [[nodiscard]] std::unique_ptr<NbtTag> clone() const override { return std::make_unique<ByteArrayTag>(*this); }
};

class StringTag : public NbtTag {
public:
    std::string value;

    StringTag() = default;
    explicit StringTag(std::string value) : value(std::move(value)) {}
    [[nodiscard]] NbtTagType getType() const noexcept override { return NbtTagType::String; }
    [[nodiscard]] std::unique_ptr<NbtTag> clone() const override { return std::make_unique<StringTag>(*this); }
};

class IntArrayTag : public NbtTag {
public:
    std::vector<int32_t> value;
    [[nodiscard]] NbtTagType getType() const noexcept override { return NbtTagType::IntArray; }
    [[nodiscard]] std::unique_ptr<NbtTag> clone() const override { return std::make_unique<IntArrayTag>(*this); }
};

class LongArrayTag : public NbtTag {
public:
    std::vector<int64_t> value;
    [[nodiscard]] NbtTagType getType() const noexcept override { return NbtTagType::LongArray; }
    [[nodiscard]] std::unique_ptr<NbtTag> clone() const override { return std::make_unique<LongArrayTag>(*this); }
};

class ListTag : public NbtTag {
public:
    NbtTagType elementType = NbtTagType::End;
    std::vector<std::unique_ptr<NbtTag>> value;

    ListTag() = default;
    explicit ListTag(NbtTagType elementType) : elementType(elementType) {}

    ListTag(const ListTag& other) : elementType(other.elementType) {
        value.reserve(other.value.size());
        for (const auto& item : other.value) {
            value.push_back(item != nullptr ? item->clone() : nullptr);
        }
    }

    ListTag& operator=(const ListTag& other) {
        if (this == &other) return *this;
        ListTag copy(other);
        elementType = copy.elementType;
        value = std::move(copy.value);
        return *this;
    }

    ListTag(ListTag&&) noexcept = default;
    ListTag& operator=(ListTag&&) noexcept = default;

    void add(std::unique_ptr<NbtTag> item) {
        if (item == nullptr) return;
        if (elementType == NbtTagType::End) elementType = item->getType();
        if (item->getType() != elementType) {
            throw std::exception{};
        }
        value.push_back(std::move(item));
    }

    [[nodiscard]] NbtTagType getType() const noexcept override { return NbtTagType::List; }
    [[nodiscard]] std::unique_ptr<NbtTag> clone() const override { return std::make_unique<ListTag>(*this); }
};

class CompoundTag : public NbtTag {
public:
    std::string name;
    std::map<std::string, std::unique_ptr<NbtTag>> value;

    CompoundTag() = default;
    explicit CompoundTag(std::string name) : name(std::move(name)) {}

    CompoundTag(const CompoundTag& other) : name(other.name) {
        for (const auto& [key, tag] : other.value) {
            value.emplace(key, tag != nullptr ? tag->clone() : nullptr);
        }
    }

    CompoundTag& operator=(const CompoundTag& other) {
        if (this == &other) return *this;
        CompoundTag copy(other);
        name = std::move(copy.name);
        value = std::move(copy.value);
        return *this;
    }

    CompoundTag(CompoundTag&&) noexcept = default;
    CompoundTag& operator=(CompoundTag&&) noexcept = default;

    void set(std::string key, std::unique_ptr<NbtTag> tag) {
        if (tag == nullptr) {
            value.erase(key);
            return;
        }
        value[std::move(key)] = std::move(tag);
    }

    template <typename Tag, typename... Args>
    Tag& emplace(std::string key, Args&&... args) {
        auto tag = std::make_unique<Tag>(std::forward<Args>(args)...);
        auto* result = tag.get();
        value[std::move(key)] = std::move(tag);
        return *result;
    }

    [[nodiscard]] NbtTag* get(std::string_view key) {
        auto it = value.find(std::string(key));
        return it == value.end() ? nullptr : it->second.get();
    }

    [[nodiscard]] const NbtTag* get(std::string_view key) const {
        auto it = value.find(std::string(key));
        return it == value.end() ? nullptr : it->second.get();
    }

    template <typename Tag>
    [[nodiscard]] Tag* get(std::string_view key) { return dynamic_cast<Tag*>(get(key)); }

    template <typename Tag>
    [[nodiscard]] const Tag* get(std::string_view key) const { return dynamic_cast<const Tag*>(get(key)); }

    [[nodiscard]] NbtTagType getType() const noexcept override { return NbtTagType::Compound; }
    [[nodiscard]] std::unique_ptr<NbtTag> clone() const override { return std::make_unique<CompoundTag>(*this); }
};

class NetworkNbtSerializer {
public:
    static std::shared_ptr<CompoundTag> read(ReadOnlyBinaryStream& stream) {
        const auto type = stream.readUnsignedByte();
        if (!type || *type != static_cast<uint8_t>(NbtTagType::Compound)) return {};

        auto name = readString(stream);
        if (!name) return {};

        auto result = std::make_shared<CompoundTag>(std::move(*name));
        if (!readCompoundPayload(stream, *result) || stream.hasOverflowed()) return {};
        return result;
    }

    static std::shared_ptr<CompoundTag> read(const std::vector<uint8_t>& encoded) {
        ReadOnlyBinaryStream stream(encoded.data(), encoded.size());
        return read(stream);
    }

    static void write(BinaryStream& stream, const std::shared_ptr<CompoundTag>& tag) {
        if (tag != nullptr) {
            write(stream, *tag);
            return;
        }
        CompoundTag empty;
        write(stream, empty);
    }

    static void write(BinaryStream& stream, const CompoundTag& tag) {
        stream.writeUnsignedByte(static_cast<uint8_t>(NbtTagType::Compound));
        stream.writeString(tag.name);
        writeCompoundPayload(stream, tag);
    }

    static std::shared_ptr<CompoundTag> readHeadless(std::string_view data, NbtTagType type, std::size_t& offset) {
        if (type != NbtTagType::Compound || offset > data.size()) {
            throw std::exception{};
        }

        ReadOnlyBinaryStream stream(data.substr(offset));
        auto result = std::make_shared<CompoundTag>();
        if (!readCompoundPayload(stream, *result) || stream.hasOverflowed()) {
            throw std::exception{};
        }
        offset += stream.position();
        return result;
    }

    static std::string writeHeadless(const std::shared_ptr<CompoundTag>& tag) {
        BinaryStream stream;
        if (tag != nullptr) {
            writeCompoundPayload(stream, *tag);
        } else {
            CompoundTag empty;
            writeCompoundPayload(stream, empty);
        }
        return stream.getBuffer();
    }

private:
    static std::optional<std::string> readString(ReadOnlyBinaryStream& stream) { return stream.readString(); }

    static bool isKnownType(uint8_t type) {
        return type <= static_cast<uint8_t>(NbtTagType::LongArray);
    }

    static std::unique_ptr<NbtTag> readPayload(ReadOnlyBinaryStream& stream, NbtTagType type) {
        switch (type) {
            case NbtTagType::Byte: {
                const auto value = stream.readByte();
                return value ? std::make_unique<ByteTag>(*value) : nullptr;
            }
            case NbtTagType::Short: {
                const auto value = stream.readShort();
                return value ? std::make_unique<ShortTag>(*value) : nullptr;
            }
            case NbtTagType::Int: {
                const auto value = stream.readVarInt();
                return value ? std::make_unique<IntTag>(*value) : nullptr;
            }
            case NbtTagType::Long: {
                const auto value = stream.readVarLong();
                return value ? std::make_unique<LongTag>(*value) : nullptr;
            }
            case NbtTagType::Float: {
                const auto value = stream.readFloat();
                return value ? std::make_unique<FloatTag>(*value) : nullptr;
            }
            case NbtTagType::Double: {
                const auto value = stream.readDouble();
                return value ? std::make_unique<DoubleTag>(*value) : nullptr;
            }
            case NbtTagType::ByteArray: {
                const auto count = stream.readVarInt();
                if (!count
                    || *count < 0
                    || static_cast<std::size_t>(*count) > stream.size() - stream.position()) {
                    return nullptr;
                }
                auto result = std::make_unique<ByteArrayTag>();
                result->value.resize(static_cast<std::size_t>(*count));
                if (!stream.readBytes(result->value.data(), result->value.size())) return nullptr;
                return result;
            }
            case NbtTagType::String: {
                auto value = readString(stream);
                return value ? std::make_unique<StringTag>(std::move(*value)) : nullptr;
            }
            case NbtTagType::List: {
                const auto elementType = stream.readUnsignedByte();
                const auto count = stream.readVarInt();
                if (!elementType || !count || !isKnownType(*elementType) || *count < 0
                    || static_cast<std::size_t>(*count) > stream.size() - stream.position()) return nullptr;

                auto result = std::make_unique<ListTag>(static_cast<NbtTagType>(*elementType));
                if (result->elementType == NbtTagType::End && *count != 0) return nullptr;
                result->value.reserve(static_cast<std::size_t>(*count));
                for (int32_t index = 0; index < *count; ++index) {
                    auto item = readPayload(stream, result->elementType);
                    if (item == nullptr) return nullptr;
                    result->value.push_back(std::move(item));
                }
                return result;
            }
            case NbtTagType::Compound: {
                auto result = std::make_unique<CompoundTag>();
                return readCompoundPayload(stream, *result) ? std::move(result) : nullptr;
            }
            case NbtTagType::IntArray: {
                const auto count = stream.readVarInt();
                if (!count || *count < 0
                    || static_cast<std::size_t>(*count) > stream.size() - stream.position()) return nullptr;
                auto result = std::make_unique<IntArrayTag>();
                result->value.reserve(static_cast<std::size_t>(*count));
                for (int32_t index = 0; index < *count; ++index) {
                    const auto value = stream.readVarInt();
                    if (!value) return nullptr;
                    result->value.push_back(*value);
                }
                return result;
            }
            case NbtTagType::LongArray: {
                const auto count = stream.readVarInt();
                if (!count || *count < 0
                    || static_cast<std::size_t>(*count) > stream.size() - stream.position()) return nullptr;
                auto result = std::make_unique<LongArrayTag>();
                result->value.reserve(static_cast<std::size_t>(*count));
                for (int32_t index = 0; index < *count; ++index) {
                    const auto value = stream.readVarLong();
                    if (!value) return nullptr;
                    result->value.push_back(*value);
                }
                return result;
            }
            case NbtTagType::End:
                return nullptr;
        }
        return nullptr;
    }

    static bool readCompoundPayload(ReadOnlyBinaryStream& stream, CompoundTag& tag) {
        while (true) {
            const auto type = stream.readUnsignedByte();
            if (!type || !isKnownType(*type)) return false;
            if (*type == static_cast<uint8_t>(NbtTagType::End)) return true;

            auto name = readString(stream);
            auto payload = name ? readPayload(stream, static_cast<NbtTagType>(*type)) : nullptr;
            if (!name || !payload) return false;
            tag.set(std::move(*name), std::move(payload));
        }
    }

    static void writePayload(BinaryStream& stream, const NbtTag& tag) {
        switch (tag.getType()) {
            case NbtTagType::Byte:
                stream.writeByte(static_cast<const ByteTag&>(tag).value);
                return;
            case NbtTagType::Short:
                stream.writeShort(static_cast<const ShortTag&>(tag).value);
                return;
            case NbtTagType::Int:
                stream.writeVarInt(static_cast<const IntTag&>(tag).value);
                return;
            case NbtTagType::Long:
                stream.writeVarLong(static_cast<const LongTag&>(tag).value);
                return;
            case NbtTagType::Float:
                stream.writeFloat(static_cast<const FloatTag&>(tag).value);
                return;
            case NbtTagType::Double:
                stream.writeDouble(static_cast<const DoubleTag&>(tag).value);
                return;
            case NbtTagType::ByteArray: {
                const auto& values = static_cast<const ByteArrayTag&>(tag).value;
                stream.writeVarInt(static_cast<int32_t>(values.size()));
                if (!values.empty()) stream.write(values.data(), values.size());
                return;
            }
            case NbtTagType::String:
                stream.writeString(static_cast<const StringTag&>(tag).value);
                return;
            case NbtTagType::List: {
                const auto& list = static_cast<const ListTag&>(tag);
                auto elementType = list.elementType;
                if (elementType == NbtTagType::End && !list.value.empty()) elementType = list.value.front()->getType();
                if (elementType != NbtTagType::End) {
                    for (const auto& item : list.value) {
                        if (!item || item->getType() != elementType) throw std::exception{};
                    }
                }
                stream.writeUnsignedByte(static_cast<uint8_t>(elementType));
                stream.writeVarInt(static_cast<int32_t>(list.value.size()));
                for (const auto& item : list.value) {
                    writePayload(stream, *item);
                }
                return;
            }
            case NbtTagType::Compound:
                writeCompoundPayload(stream, static_cast<const CompoundTag&>(tag));
                return;
            case NbtTagType::IntArray: {
                const auto& values = static_cast<const IntArrayTag&>(tag).value;
                stream.writeVarInt(static_cast<int32_t>(values.size()));
                for (const auto value : values) {
                    stream.writeVarInt(value);
                }
                return;
            }
            case NbtTagType::LongArray: {
                const auto& values = static_cast<const LongArrayTag&>(tag).value;
                stream.writeVarInt(static_cast<int32_t>(values.size()));
                for (const auto value : values) {
                    stream.writeVarLong(value);
                }
                return;
            }
            case NbtTagType::End:
                throw std::exception{};
        }
    }

    static void writeCompoundPayload(BinaryStream& stream, const CompoundTag& tag) {
        for (const auto& [name, child] : tag.value) {
            if (!child || child->getType() == NbtTagType::End) continue;
            stream.writeUnsignedByte(static_cast<uint8_t>(child->getType()));
            stream.writeString(name);
            writePayload(stream, *child);
        }
        stream.writeUnsignedByte(static_cast<uint8_t>(NbtTagType::End));
    }
};

} // namespace bedrock::protocol
