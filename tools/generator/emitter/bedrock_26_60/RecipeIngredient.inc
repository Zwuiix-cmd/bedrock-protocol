<?php

declare(strict_types=1);

return [
    ['#include <cstdint>', '#include <bedrock/protocol/types/recipe/ItemDescriptorType.hpp>' . "\n" . '#include <cstdint>'],
    ['uint16_t count;', 'int32_t count = 0;'],
    ['const uint16_t& countValue', 'const int32_t& countValue'],
    [<<<'BEFORE'
    virtual void read(ReadOnlyBinaryStream& stream) {
        descriptor = stream.readOptionalVariant<std::variant<std::monostate, std::shared_ptr<StringIdMetaItemDescriptor>, std::shared_ptr<TagItemDescriptor>, std::shared_ptr<MolangItemDescriptor>>>([&]() { return stream.readObject<StringIdMetaItemDescriptor>(); });
        count = stream.readUnsignedShort().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        std::visit([&](const auto& alternative) { using Alternative = std::remove_cvref_t<decltype(alternative)>; if constexpr (std::is_same_v<Alternative, std::shared_ptr<StringIdMetaItemDescriptor>>) { alternative->write(stream); } else if constexpr (std::is_same_v<Alternative, std::shared_ptr<TagItemDescriptor>>) { alternative->write(stream); } else if constexpr (std::is_same_v<Alternative, std::shared_ptr<MolangItemDescriptor>>) { alternative->write(stream); } }, descriptor);
        stream.writeUnsignedShort(count);
    }
BEFORE,
<<<'AFTER'
    virtual void read(ReadOnlyBinaryStream& stream) {
        const auto variant = stream.readUnsignedByte().value_or(0);
        if (variant == 0) {
            descriptor = std::monostate{};
            if (stream.readVarInt().value_or(0) != 32767) {
                stream.markOverflowed();
                return;
            }
        } else if (variant == 1) {
            const auto type = magic_enum::enum_cast<ItemDescriptorType>(stream.readString().value_or(""));
            if (!type) {
                stream.markOverflowed();
                return;
            }
            readDescriptor(stream, *type, false);
        } else {
            stream.markOverflowed();
            return;
        }
        count = stream.readVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        const auto type = descriptorType();
        if (type == ItemDescriptorType::Empty) {
            stream.writeUnsignedByte(0);
            stream.writeVarInt(32767);
        } else {
            stream.writeUnsignedByte(1);
            stream.writeString(magic_enum::enum_name(type));
            writeDescriptor(stream, false);
        }
        stream.writeVarInt(count);
    }

    void readStackRequest(ReadOnlyBinaryStream& stream) {
        const auto type = stream.readUnsignedVarInt().value_or(0);
        const auto innerType = stream.readUnsignedByte().value_or(0);
        if (type != innerType || type > magic_enum::enum_integer(ItemDescriptorType::Tag)) {
            stream.markOverflowed();
            return;
        }
        readDescriptor(stream, static_cast<ItemDescriptorType>(type), true);
        count = stream.readUnsignedShort().value_or(0);
    }

    void writeStackRequest(BinaryStream& stream) const {
        if (count < 0 || count > UINT16_MAX) {
            throw std::out_of_range("Stack request ingredient count is out of range");
        }
        const auto type = magic_enum::enum_integer(descriptorType());
        stream.writeUnsignedVarInt(type);
        stream.writeUnsignedByte(static_cast<uint8_t>(type));
        writeDescriptor(stream, true);
        stream.writeUnsignedShort(static_cast<uint16_t>(count));
    }

private:
    [[nodiscard]] ItemDescriptorType descriptorType() const {
        if (const auto* value = std::get_if<std::shared_ptr<StringIdMetaItemDescriptor>>(&descriptor); value && *value) {
            return ItemDescriptorType::StringIdMeta;
        }
        if (const auto* value = std::get_if<std::shared_ptr<TagItemDescriptor>>(&descriptor); value && *value) {
            return ItemDescriptorType::Tag;
        }
        if (const auto* value = std::get_if<std::shared_ptr<MolangItemDescriptor>>(&descriptor); value && *value) {
            return ItemDescriptorType::Molang;
        }
        return ItemDescriptorType::Empty;
    }

    void readDescriptor(ReadOnlyBinaryStream& stream, ItemDescriptorType type, bool tagOnly) {
        switch (type) {
            case ItemDescriptorType::Empty:
                descriptor = std::monostate{};
                break;
            case ItemDescriptorType::StringIdMeta:
                descriptor = stream.readObject<StringIdMetaItemDescriptor>();
                break;
            case ItemDescriptorType::Tag:
                if (tagOnly) {
                    descriptor = std::make_shared<TagItemDescriptor>(
                        stream.readString().value_or(""), TagItemDescriptor::DEFAULT_META);
                } else {
                    descriptor = stream.readObject<TagItemDescriptor>();
                }
                break;
            case ItemDescriptorType::Molang:
                descriptor = stream.readObject<MolangItemDescriptor>();
                break;
        }
    }

    void writeDescriptor(BinaryStream& stream, bool tagOnly) const {
        std::visit([&](const auto& value) {
            using Value = std::remove_cvref_t<decltype(value)>;
            if constexpr (!std::is_same_v<Value, std::monostate>) {
                if (!value) {
                    return;
                }
                if constexpr (std::is_same_v<Value, std::shared_ptr<TagItemDescriptor>>) {
                    if (tagOnly) {
                        stream.writeString(value->tag);
                        return;
                    }
                }
                value->write(stream);
            }
        }, descriptor);
    }
AFTER],
];
