<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
return stream.readOptional([&]() { return DynamicValue::create(stream); });
BEFORE,
<<<'AFTER'
return DynamicValue::readTyped(stream);
AFTER],
    [<<<'BEFORE'
stream.writeOptional(item, [&](const auto& optionalValue) { optionalValue->write(stream); });
BEFORE,
<<<'AFTER'
DynamicValue::writeTyped(stream, item);
AFTER],
];
