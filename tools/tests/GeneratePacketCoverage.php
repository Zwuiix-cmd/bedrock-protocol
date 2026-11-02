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
    '#include <optional>',
    '#include <bedrock/protocol/PacketIds.hpp>',
    '#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>',
    '#include <bedrock/protocol/pool/PacketPool.hpp>',
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
    '        const auto packet = PacketPool::createPacket(id);',
    '        expect(packet != nullptr, "generated packet was not registered");',
    '        if (!packet) {',
    '            continue;',
    '        }',
    '        expect(packet->getId() == id, "generated packet has the wrong ID");',
    '',
    '        ReadOnlyBinaryStream input;',
    '        try {',
    '            packet->read(input);',
    '        } catch (const std::bad_optional_access&) {',
    '            expect(false, "packet read raised std::bad_optional_access");',
    '        } catch (...) {',
    '        }',
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
