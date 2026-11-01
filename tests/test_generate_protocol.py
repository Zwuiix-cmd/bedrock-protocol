from __future__ import annotations

import sys
import unittest
from pathlib import Path


TOOLS = Path(__file__).resolve().parents[1] / "tools"
sys.path.insert(0, str(TOOLS))

from generate_protocol import (  # noqa: E402
    parse_create_parameters,
    parse_php_scalar,
    parse_protocol_constants,
    split_top_level,
)


class GeneratorParsingTests(unittest.TestCase):
    def test_parse_php_scalars(self) -> None:
        self.assertEqual(parse_php_scalar("0x13"), 19)
        self.assertEqual(parse_php_scalar("2_225"), 2225)
        self.assertEqual(parse_php_scalar("'v26.60'"), "v26.60")
        self.assertIs(parse_php_scalar("false"), False)

    def test_split_top_level_ignores_nested_commas(self) -> None:
        self.assertEqual(
            split_top_level("int $id, array $values = [1, 2], string $name"),
            ["int $id", "array $values = [1, 2]", "string $name"],
        )

    def test_parse_create_parameters(self) -> None:
        source = """
        public static function create(
            int $actorRuntimeId,
            ?MovePlayerTeleportData $telemetryData,
            array $values = [],
        ) : self{
            return new self;
        }
        """
        parameters = parse_create_parameters(source)
        self.assertEqual([parameter.name for parameter in parameters], [
            "actorRuntimeId",
            "telemetryData",
            "values",
        ])
        self.assertEqual(parameters[1].php_type, "?MovePlayerTeleportData")
        self.assertEqual(parameters[2].default, "[]")

    def test_parse_protocol_constants(self) -> None:
        path = Path(__file__).with_name("ProtocolInfo.fixture.php")
        path.write_text(
            """<?php\npublic const CURRENT_PROTOCOL = 2_225;\npublic const LOGIN_PACKET = 0x01;\npublic const MINECRAFT_VERSION = 'v26.60';\n""",
            encoding="utf-8",
        )
        try:
            constants = parse_protocol_constants(path)
        finally:
            path.unlink()
        self.assertEqual(constants["CURRENT_PROTOCOL"], 2225)
        self.assertEqual(constants["LOGIN_PACKET"], 1)
        self.assertEqual(constants["MINECRAFT_VERSION"], "v26.60")


if __name__ == "__main__":
    unittest.main()
