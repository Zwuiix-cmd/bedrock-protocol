<?php

declare(strict_types=1);

return [
    ['ingredients = stream.readObjectList<RecipeIngredient>();', <<<'CPP'
ingredients = stream.readList([&]() {
            auto ingredient = std::make_shared<RecipeIngredient>();
            ingredient->readStackRequest(stream);
            return ingredient;
        });
CPP],
    ['item->write(stream);', 'item->writeStackRequest(stream);'],
];
