<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
vehicleInfo = nullptr;
BEFORE,
<<<'AFTER'
vehicleInfo.reset();
AFTER],
    [<<<'BEFORE'
magic_enum::enum_integer(PlayerAuthInputFlags::Count)
BEFORE,
<<<'AFTER'
PlayerAuthInputFlags::Count
AFTER],
];
