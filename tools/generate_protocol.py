#!/usr/bin/env python3
"""Extract Bedrock protocol metadata from the upstream PHP repository.

The first generator stage intentionally emits metadata and registration
descriptors only. Packet serialization code will be added after the PHP type
mapping has been validated against Kodiak's existing packet model.
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Iterable


GENERATOR_VERSION = "0.1.0"
UPSTREAM_REPOSITORY = "https://github.com/axolotl-pm/BedrockProtocol.git"


@dataclass(frozen=True)
class CreateParameter:
    name: str
    php_type: str
    default: str | None

    def as_json(self) -> dict[str, str | None]:
        return {
            "name": self.name,
            "type": self.php_type,
            "default": self.default,
        }


@dataclass(frozen=True)
class Packet:
    class_name: str
    parent_class: str
    source: str
    network_constant: str | None
    network_id: int | None
    interfaces: tuple[str, ...]
    fields: tuple[dict[str, str | None], ...]
    create_parameters: tuple[CreateParameter, ...]


def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8")


def run_git(source: Path, *arguments: str) -> str | None:
    try:
        result = subprocess.run(
            ["git", "-C", str(source), *arguments],
            check=True,
            capture_output=True,
            text=True,
        )
    except (OSError, subprocess.CalledProcessError):
        return None
    return result.stdout.strip()


def source_revision(source: Path) -> dict[str, str | None]:
    return {
        "repository": UPSTREAM_REPOSITORY,
        "branch": run_git(source, "branch", "--show-current") or None,
        "commit": run_git(source, "rev-parse", "HEAD"),
        "commitDate": run_git(source, "show", "-s", "--format=%cI", "HEAD"),
    }


def parse_php_scalar(expression: str) -> int | str | bool | None:
    value = expression.strip().replace("_", "")
    if re.fullmatch(r"-?0[xX][0-9a-fA-F]+", value):
        return int(value, 16)
    if re.fullmatch(r"-?[0-9]+", value):
        return int(value, 10)
    if value == "true":
        return True
    if value == "false":
        return False
    if value == "null":
        return None
    if len(value) >= 2 and value[0] == value[-1] and value[0] in "'\"":
        return value[1:-1]
    return value


def parse_protocol_constants(path: Path) -> dict[str, int | str | bool | None]:
    pattern = re.compile(
        r"(?m)^\s*public\s+const\s+(?P<name>[A-Z][A-Z0-9_]*)\s*="
        r"\s*(?P<value>[^;]+);"
    )
    return {
        match.group("name"): parse_php_scalar(match.group("value"))
        for match in pattern.finditer(read_text(path))
    }


def split_top_level(value: str, delimiter: str = ",") -> list[str]:
    parts: list[str] = []
    start = 0
    depth = 0
    quote: str | None = None
    escaped = False

    for index, character in enumerate(value):
        if quote is not None:
            if escaped:
                escaped = False
            elif character == "\\":
                escaped = True
            elif character == quote:
                quote = None
            continue

        if character in "'\"":
            quote = character
        elif character in "([{":
            depth += 1
        elif character in ")]}":
            depth -= 1
        elif character == delimiter and depth == 0:
            parts.append(value[start:index].strip())
            start = index + 1

    tail = value[start:].strip()
    if tail:
        parts.append(tail)
    return parts


def parenthesized_content(source: str, opening: int) -> str:
    depth = 0
    quote: str | None = None
    escaped = False

    for index in range(opening, len(source)):
        character = source[index]
        if quote is not None:
            if escaped:
                escaped = False
            elif character == "\\":
                escaped = True
            elif character == quote:
                quote = None
            continue

        if character in "'\"":
            quote = character
        elif character == "(":
            depth += 1
        elif character == ")":
            depth -= 1
            if depth == 0:
                return source[opening + 1:index]

    raise ValueError("unterminated PHP parameter list")


def split_parameter_default(parameter: str) -> tuple[str, str | None]:
    depth = 0
    quote: str | None = None
    escaped = False

    for index, character in enumerate(parameter):
        if quote is not None:
            if escaped:
                escaped = False
            elif character == "\\":
                escaped = True
            elif character == quote:
                quote = None
            continue

        if character in "'\"":
            quote = character
        elif character in "([{":
            depth += 1
        elif character in ")]}":
            depth -= 1
        elif character == "=" and depth == 0:
            if index + 1 < len(parameter) and parameter[index + 1] == ">":
                continue
            return parameter[:index].strip(), parameter[index + 1 :].strip()

    return parameter.strip(), None


def parse_create_parameters(source: str) -> tuple[CreateParameter, ...]:
    opening: int | None = None
    for function_name in ("create", "internalCreate"):
        match = re.search(rf"\bfunction\s+{function_name}\s*\(", source)
        if match is not None:
            opening = source.find("(", match.start())
            break

    if opening is None:
        return ()

    content = parenthesized_content(source, opening)
    parameters: list[CreateParameter] = []
    for raw_parameter in split_top_level(content):
        raw_parameter = re.sub(r"/\*.*?\*/", "", raw_parameter, flags=re.DOTALL).strip()
        if not raw_parameter:
            continue

        declaration, default = split_parameter_default(raw_parameter)
        name_match = re.search(r"(?:\.\.\.)?\$(?P<name>[A-Za-z_]\w*)$", declaration)
        if name_match is None:
            continue

        php_type = declaration[: name_match.start()].replace("&", "").replace("...", "").strip()
        parameters.append(
            CreateParameter(
                name=name_match.group("name"),
                php_type=php_type or "mixed",
                default=default,
            )
        )
    return tuple(parameters)


def parse_public_fields(source: str) -> tuple[dict[str, str | None], ...]:
    pattern = re.compile(
        r"(?m)^\s*public\s+(?!const\b|static\s+function\b)"
        r"(?P<type>[^$;\n]+?)\s+\$(?P<name>[A-Za-z_]\w*)"
        r"(?:\s*=\s*(?P<default>[^;]+))?;"
    )
    fields: list[dict[str, str | None]] = []
    for match in pattern.finditer(source):
        php_type = match.group("type").replace("readonly", "").strip()
        fields.append(
            {
                "name": match.group("name"),
                "type": php_type,
                "default": match.group("default").strip() if match.group("default") else None,
            }
        )
    return tuple(fields)


def parse_packet(
    path: Path,
    source_root: Path,
    protocol_constants: dict[str, int | str | bool | None],
) -> Packet | None:
    source = read_text(path)
    class_match = re.search(
        r"\bclass\s+(?P<class>[A-Za-z_]\w*)\s+extends\s+(?P<parent>[A-Za-z_\\]\w*)"
        r"(?:\s+implements\s+(?P<interfaces>[^\{]+))?\s*\{",
        source,
    )
    if class_match is None:
        return None

    network_match = re.search(
        r"public\s+const\s+NETWORK_ID\s*=\s*(?P<value>[^;]+);", source
    )
    network_constant: str | None = None
    network_id: int | None = None
    if network_match is not None:
        value = network_match.group("value").strip()
        reference = re.fullmatch(r"ProtocolInfo::([A-Z0-9_]+)", value)
        if reference is not None:
            network_constant = reference.group(1)
            candidate = protocol_constants.get(network_constant)
            if isinstance(candidate, int):
                network_id = candidate
        else:
            candidate = parse_php_scalar(value)
            if isinstance(candidate, int):
                network_id = candidate

    interfaces = tuple(
        interface.strip()
        for interface in (class_match.group("interfaces") or "").split(",")
        if interface.strip()
    )
    return Packet(
        class_name=class_match.group("class"),
        parent_class=class_match.group("parent"),
        source=path.relative_to(source_root).as_posix(),
        network_constant=network_constant,
        network_id=network_id,
        interfaces=interfaces,
        fields=parse_public_fields(source),
        create_parameters=parse_create_parameters(source),
    )


def parse_packets(
    source_root: Path,
    protocol_constants: dict[str, int | str | bool | None],
) -> list[Packet]:
    candidates = [
        packet
        for path in sorted((source_root / "src").rglob("*.php"))
        if (packet := parse_packet(path, source_root / "src", protocol_constants)) is not None
    ]
    by_class = {packet.class_name: packet for packet in candidates}
    packet_cache: dict[str, bool] = {}

    def inherits_data_packet(packet: Packet, active: set[str] | None = None) -> bool:
        cached = packet_cache.get(packet.class_name)
        if cached is not None:
            return cached
        active = set() if active is None else active
        if packet.class_name in active:
            return False
        active.add(packet.class_name)
        if packet.parent_class == "DataPacket":
            result = True
        else:
            parent = by_class.get(packet.parent_class)
            result = parent is not None and inherits_data_packet(parent, active)
        packet_cache[packet.class_name] = result
        return result

    packets = [packet for packet in candidates if inherits_data_packet(packet)]
    return sorted(packets, key=lambda packet: packet.class_name)


def parse_packet_pool(source_root: Path) -> list[str]:
    source = read_text(source_root / "src" / "PacketPool.php")
    return re.findall(r"registerPacket\s*\(\s*new\s+([A-Za-z_]\w*)\s*\(\s*\)\s*\)", source)


def packet_id_name(constant: str) -> str:
    name = constant.removesuffix("_PACKET")
    return "".join(part[:1].upper() + part[1:].lower() for part in name.split("_") if part)


def packet_model(
    source_root: Path,
    source_label: str,
) -> dict[str, Any]:
    protocol_constants = parse_protocol_constants(source_root / "src" / "ProtocolInfo.php")
    packet_id_constants = {
        name: value
        for name, value in protocol_constants.items()
        if name.endswith("_PACKET") and isinstance(value, int)
    }
    packets = parse_packets(source_root, protocol_constants)
    packet_by_class = {packet.class_name: packet for packet in packets}
    packet_by_constant = {
        packet.network_constant: packet
        for packet in packets
        if packet.network_constant is not None
    }
    pool_classes = parse_packet_pool(source_root)

    warnings: list[str] = []
    for packet in packets:
        if packet.network_id is None:
            warnings.append(f"{packet.class_name}: unresolved NETWORK_ID")
        elif packet.network_constant not in packet_id_constants:
            warnings.append(f"{packet.class_name}: NETWORK_ID is not in ProtocolInfo")
    for class_name in pool_classes:
        if class_name not in packet_by_class:
            warnings.append(f"PacketPool registers unknown packet class {class_name}")

    protocol_id_rows = []
    for constant, network_id in sorted(packet_id_constants.items(), key=lambda item: (item[1], item[0])):
        packet = packet_by_constant.get(constant)
        protocol_id_rows.append(
            {
                "constant": constant,
                "name": packet_id_name(constant),
                "id": network_id,
                "hex": f"0x{network_id:02X}",
                "packetClass": packet.class_name if packet else None,
            }
        )

    packet_rows = []
    for packet in packets:
        packet_rows.append(
            {
                "class": packet.class_name,
                "parent": packet.parent_class,
                "source": packet.source,
                "networkConstant": packet.network_constant,
                "networkId": packet.network_id,
                "networkIdHex": f"0x{packet.network_id:02X}" if packet.network_id is not None else None,
                "directions": list(packet.interfaces),
                "fields": list(packet.fields),
                "createParameters": [parameter.as_json() for parameter in packet.create_parameters],
                "registered": packet.class_name in pool_classes,
            }
        )

    pool_rows = []
    for order, class_name in enumerate(pool_classes):
        packet = packet_by_class.get(class_name)
        pool_rows.append(
            {
                "order": order,
                "class": class_name,
                "networkConstant": packet.network_constant if packet else None,
                "networkId": packet.network_id if packet else None,
            }
        )

    return {
        "schemaVersion": 1,
        "generator": {
            "name": "kodiak-bedrock-protocol",
            "version": GENERATOR_VERSION,
        },
        "source": {
            **source_revision(source_root),
            "path": source_label,
        },
        "protocol": {
            "currentProtocol": protocol_constants.get("CURRENT_PROTOCOL"),
            "minecraftVersion": protocol_constants.get("MINECRAFT_VERSION"),
            "minecraftVersionNetwork": protocol_constants.get("MINECRAFT_VERSION_NETWORK"),
        },
        "packetIds": protocol_id_rows,
        "packets": packet_rows,
        "packetPool": pool_rows,
        "warnings": sorted(set(warnings)),
    }


def write_text_or_check(path: Path, content: str, check: bool) -> bool:
    if check:
        return path.exists() and path.read_text(encoding="utf-8") == content
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8", newline="\n")
    return True


def cpp_header_preamble(model: dict[str, Any]) -> list[str]:
    source = model["source"]
    return [
        "// Generated file. Do not edit manually.",
        f"// Source: {source['repository']}",
        f"// Source commit: {source['commit']}",
        "",
        "#pragma once",
        "",
        "#include <array>",
        "#include <cstdint>",
        "",
        "namespace kodiak::bedrock::protocol {",
        "",
    ]


def emit_packet_ids(model: dict[str, Any]) -> str:
    rows = model["packetIds"]
    lines = cpp_header_preamble(model)
    lines.extend(["enum class PacketId : std::uint16_t {",])
    for row in rows:
        lines.append(f"    {row['name']} = {row['hex']},")
    lines.extend([
        "};",
        "",
        "struct PacketIdInfo {",
        "    PacketId id;",
        "    std::uint16_t value;",
        "    const char* constant;",
        "    const char* packetClass;",
        "};",
        "",
        f"inline constexpr std::array<PacketIdInfo, {len(rows)}> packetIds = {{{{",
    ])
    for row in rows:
        packet_class = row["packetClass"] or ""
        lines.append(
            f"    {{PacketId::{row['name']}, {row['hex']}, \"{row['constant']}\", \"{packet_class}\"}},"
        )
    lines.extend([
        "}};",
        "",
        "constexpr std::uint16_t toValue(PacketId id) noexcept {",
        "    return static_cast<std::uint16_t>(id);",
        "}",
        "",
        "} // namespace kodiak::bedrock::protocol",
        "",
    ])
    return "\n".join(lines)


def emit_packet_pool(model: dict[str, Any]) -> str:
    rows = model["packetPool"]
    valid_rows = [
        row for row in rows
        if row["networkConstant"] is not None and row["networkId"] is not None
    ]
    lines = cpp_header_preamble(model)
    lines.insert(lines.index("#include <array>"), '#include "PacketIds.hpp"')
    lines.extend([
        "struct PacketPoolEntry {",
        "    PacketId id;",
        "    std::uint16_t value;",
        "    const char* packetClass;",
        "};",
        "",
        f"inline constexpr std::array<PacketPoolEntry, {len(valid_rows)}> packetPool = {{{{",
    ])
    for row in valid_rows:
        constant = row["networkConstant"]
        lines.append(
            f"    {{PacketId::{packet_id_name(constant)}, 0x{row['networkId']:02X}, \"{row['class']}\"}},"
        )
    lines.extend([
        "}};",
        "",
        "} // namespace kodiak::bedrock::protocol",
        "",
    ])
    return "\n".join(lines)


def generate(output: Path, model: dict[str, Any], check: bool) -> bool:
    metadata = json.dumps(model, indent=2, ensure_ascii=False) + "\n"
    artifacts = {
        output / "metadata" / "protocol.json": metadata,
        output / "cpp" / "PacketIds.hpp": emit_packet_ids(model),
        output / "cpp" / "PacketPool.hpp": emit_packet_pool(model),
    }
    return all(write_text_or_check(path, content, check) for path, content in artifacts.items())


def parse_arguments() -> argparse.Namespace:
    repository_root = Path(__file__).resolve().parents[1]
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--source",
        type=Path,
        default=repository_root / "upstream" / "BedrockProtocol",
        help="Path to the checked-out upstream BedrockProtocol repository",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=repository_root / "generated",
        help="Directory receiving metadata and generated C++ headers",
    )
    parser.add_argument("--strict", action="store_true", help="Fail when the parser reports warnings")
    parser.add_argument("--check", action="store_true", help="Check generated files without writing them")
    return parser.parse_args()


def main() -> int:
    arguments = parse_arguments()
    repository_root = Path(__file__).resolve().parents[1]
    source = arguments.source.resolve()
    output = arguments.output.resolve()

    required_files = [source / "src" / "ProtocolInfo.php", source / "src" / "PacketPool.php"]
    missing = [str(path) for path in required_files if not path.is_file()]
    if missing:
        print("Missing upstream files:", file=sys.stderr)
        print("\n".join(missing), file=sys.stderr)
        return 2

    try:
        try:
            source_label = source.relative_to(repository_root).as_posix()
        except ValueError:
            source_label = arguments.source.as_posix()
        model = packet_model(source, source_label)
    except (OSError, ValueError) as error:
        print(f"Generation failed: {error}", file=sys.stderr)
        return 2

    warnings = model["warnings"]
    for warning in warnings:
        print(f"warning: {warning}", file=sys.stderr)
    if arguments.strict and warnings:
        print("Strict mode refused to generate output.", file=sys.stderr)
        return 1

    if not generate(output, model, arguments.check):
        print("Generated output is stale.", file=sys.stderr)
        return 1

    source_commit = model["source"]["commit"] or "unknown"
    action = "checked" if arguments.check else "generated"
    print(
        f"{action}: {len(model['packetIds'])} packet IDs, "
        f"{len(model['packets'])} packet classes, "
        f"{len(model['packetPool'])} pool registrations "
        f"from {source_commit}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
