<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
		if ((unlockingContextValue != RecipeUnlockingContext::None) && (unlockingIngredientsValue != nullptr)) {
BEFORE,
<<<'AFTER'
        if (unlockingContextValue != RecipeUnlockingContext::None && unlockingIngredientsValue.has_value()) {
AFTER],
];
