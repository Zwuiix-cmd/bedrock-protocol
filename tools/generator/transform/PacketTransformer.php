<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\transform;

final class PacketTransformer extends ModelTransformer
{
    /** @param array<string, mixed> $model @return array<string, mixed> */
    protected function apply(array $model): array
    {
        $packets = [];
        foreach ($model['packets'] ?? [] as $packet) {
            if (!is_array($packet) || !isset($packet['name'])) {
                continue;
            }
            $packet['networkId'] = (int) ($packet['networkId'] ?? 0);
            $packets[] = $packet;
        }

        usort($packets, static fn (array $left, array $right): int => $left['networkId'] <=> $right['networkId']);
        $model['packets'] = $packets;

        $registrations = [];
        foreach ($model['registrations'] ?? [] as $registration) {
            if (!is_string($registration) || $registration === '' || in_array($registration, $registrations, true)) {
                continue;
            }
            $registrations[] = $registration;
        }
        $model['registrations'] = $registrations;

        return $model;
    }
}
