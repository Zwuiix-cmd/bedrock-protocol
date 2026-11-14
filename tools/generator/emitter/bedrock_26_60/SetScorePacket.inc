<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
entry->action = magic_enum::enum_cast<ScorePacketEntryAction>(
				action
			).value_or(ScorePacketEntryAction::Remove);
BEFORE,
<<<'AFTER'
entry->action = action;
AFTER],
];
