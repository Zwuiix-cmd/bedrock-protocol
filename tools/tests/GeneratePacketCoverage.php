<?php

declare(strict_types=1);

$root = dirname(__DIR__, 2);
$packetIdsPath = $root . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock' . DIRECTORY_SEPARATOR . 'protocol' . DIRECTORY_SEPARATOR . 'PacketIds.hpp';
$outputPath = $root . DIRECTORY_SEPARATOR . 'tests' . DIRECTORY_SEPARATOR . 'GeneratedPacketCoverage.hpp';
$contents = file_get_contents($packetIdsPath);
if ($contents === false) {
    fwrite(STDERR, "Unable to read PacketIds.hpp\n");
    exit(1);
}

preg_match_all('/^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(-?\d+)\s*,/m', $contents, $matches, PREG_SET_ORDER);
$packets = [];
foreach ($matches as $match) {
    if ($match[1] !== 'Unknown') {
        $packets[] = $match[1];
    }
}
if ($packets === []) {
    fwrite(STDERR, "PacketIds.hpp does not contain any packet\n");
    exit(1);
}

$lines = [
    '#pragma once',
    '',
    '#include <array>',
    '#include <cstdint>',
    '#include <exception>',
    '#include <optional>',
    '#include <string>',
    '#include <bedrock/protocol/PacketIds.hpp>',
    '#include <bedrock/protocol/io/BinaryStream.hpp>',
    '#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>',
    '#include <bedrock/protocol/pool/PacketPool.hpp>',

    '#include <magic_enum/magic_enum.hpp>',
    '',
    'namespace generated_packet_tests {',
    '',
    'template <typename Expect>',
    'void run(Expect&& expect) {',
    '    PacketPool::setup();',
    '    constexpr std::array packetIds{',
];
foreach ($packets as $packet) {
    $lines[] = '        PacketIds::' . $packet . ',';
}
$lines = array_merge($lines, [
    '    };',
    '',
    '    for (const auto id : packetIds) {',
    '        const auto packetName = std::string("packet ") + std::to_string(static_cast<int>(id));',
    '        const auto packet = PacketPool::createPacket(id);',
    '        expect(packet != nullptr, std::string(packetName) + ": packet was not registered");',
    '        if (!packet) {',
    '            continue;',
    '        }',
    '        expect(packet->getId() == id, std::string(packetName) + ": packet has the wrong ID");',
    '',
    '        bool roundtripSucceeded = false;',
    '        for (const auto fill : std::array<uint8_t, 5>{0, 1, 2, 9, 17}) {',
    '            const auto candidate = PacketPool::createPacket(id);',
    '            const std::string fixture(4096, static_cast<char>(fill));',
    '            ReadOnlyBinaryStream input(fixture);',
    '            try {',
    '                candidate->read(input);',
    '                if (input.hasOverflowed()) {',
    '                    continue;',
    '                }',
    '                BinaryStream output;',
    '                candidate->write(output);',
    '                roundtripSucceeded = true;',
    '                break;',
    '            } catch (const std::bad_optional_access&) {',
    '                expect(false, std::string(packetName) + ": read raised std::bad_optional_access");',
    '            } catch (const std::exception&) {',
    '            } catch (...) {',
    '            }',
    '        }',
    '        expect(roundtripSucceeded, std::string(packetName) + ": no fixture could be decoded and encoded");',
    '    }',
    '}',
    '',
    '} // namespace generated_packet_tests',
    '',
]);

if (file_put_contents($outputPath, implode(PHP_EOL, $lines)) === false) {
    fwrite(STDERR, "Unable to write generated packet coverage\n");
    exit(1);
}
printf("generated packet coverage for %d packets\n", count($packets));
