<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
seed = stream.readUnsignedLong().value_or(0);
BEFORE,
<<<'AFTER'
seed = stream.readUnsignedLong().value_or(0);
        spawnSettings = stream.readObject<SpawnSettings>();
AFTER],
    [<<<'BEFORE'
isTexturePacksRequired = stream.readBool().value_or(false);
BEFORE,
<<<'AFTER'
isTexturePacksRequired = stream.readBool().value_or(false);
        gameRules = GameRule::readRules(stream, false);
        experiments = stream.readObject<Experiments>();
AFTER],
    [<<<'BEFORE'
isNewNether = stream.readBool().value_or(false);
BEFORE,
<<<'AFTER'
isNewNether = stream.readBool().value_or(false);
        eduSharedUriResource = stream.readObject<EducationUriResource>();
AFTER],
    [<<<'BEFORE'
spawnSettings = std::make_shared<SpawnSettings>();
        spawnSettings->read(stream);
        const auto gameRulesCount = stream.readUnsignedInt().value_or(0);
        const bool gameRulesCountAvailable = stream.canReadCollection(gameRulesCount);
        for (std::size_t index = 0; index < gameRulesCount && gameRulesCountAvailable && !stream.hasOverflowed(); ++index) {
            auto gameRulesKey = stream.readString().value_or("");
            auto gameRulesValue = GameRule::create(stream);
            gameRules.emplace(std::move(gameRulesKey), std::move(gameRulesValue));
        }
        experiments = std::make_shared<Experiments>();
        experiments->read(stream);
        eduSharedUriResource = stream.readOptional([&]() {
            return stream.readObject<EducationUriResource>();
        });
BEFORE,
<<<'AFTER'

AFTER],
];
