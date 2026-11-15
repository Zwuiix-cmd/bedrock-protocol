<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
static std::shared_ptr<ItemStackRequestAction> create(ReadOnlyBinaryStream& stream);
BEFORE,
<<<'AFTER'
static std::shared_ptr<ItemStackRequestAction> create(ReadOnlyBinaryStream& stream);
    static std::shared_ptr<ItemStackRequestAction> create(ReadOnlyBinaryStream& stream, uint32_t type);
AFTER],
];
