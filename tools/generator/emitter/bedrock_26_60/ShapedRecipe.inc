<?php

declare(strict_types=1);

return [
    ['input = stream.readList([&]() { return stream.readObjectList<RecipeIngredient>(); });', <<<'CPP'
const auto width = stream.readVarInt().value_or(0);
        const auto height = stream.readVarInt().value_or(0);
        const auto count = stream.readUnsignedVarInt().value_or(0);
        if (width < 0 || height < 1 || height > 3
            || count != static_cast<uint64_t>(width) * height
            || !stream.canReadCollection(count)) {
            stream.markOverflowed();
            return;
        }
        input.assign(static_cast<std::size_t>(height), {});
        for (auto& row : input) {
            row.reserve(static_cast<std::size_t>(width));
            for (int32_t column = 0; column < width && !stream.hasOverflowed(); ++column) {
                row.push_back(stream.readObject<RecipeIngredient>());
            }
        }
CPP],
    ['stream.writeList(input, [&](const auto& item) { stream.writeList(item, [&](const auto& item) { item->write(stream); }); });', <<<'CPP'
if (input.empty() || input.size() > 3) {
            throw std::invalid_argument("Invalid shaped recipe height");
        }
        const auto width = input.front().size();
        stream.writeVarInt(static_cast<int32_t>(width));
        stream.writeVarInt(static_cast<int32_t>(input.size()));
        stream.writeUnsignedVarInt(static_cast<uint32_t>(width * input.size()));
        for (const auto& row : input) {
            if (row.size() != width) {
                throw std::invalid_argument("Inconsistent shaped recipe row width");
            }
            for (const auto& ingredient : row) {
                ingredient->write(stream);
            }
        }
CPP],
    ['uuid = stream.readString().value_or("");', "uuid = stream.readUUID().value_or(\"\");\n        blockName = stream.readString().value_or(\"\");"],
    ['recipeNetId = stream.readUnsignedVarInt().value_or(0); blockName = stream.readString().value_or("");', 'recipeNetId = stream.readUnsignedVarInt().value_or(0);'],
    ['stream.writeString(uuid);', "stream.writeUUID(uuid);\n        stream.writeString(blockName);"],
    ['stream.writeUnsignedVarInt(recipeNetId); stream.writeString(blockName);', 'stream.writeUnsignedVarInt(recipeNetId);'],
];
