<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\emitter;

use kodiak\bedrockprotocol\generator\Configuration;
use kodiak\bedrockprotocol\generator\CPPName;
use kodiak\bedrockprotocol\generator\CPP\CPPDefaultValueResolver;
use RuntimeException;

trait RuntimeSupportEmissionMethods
{
    private function writeRuntime(string $output): void
    {
        $base = $output . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock' . DIRECTORY_SEPARATOR . 'protocol';
        $this->write($base . DIRECTORY_SEPARATOR . 'Packet.hpp', <<<'CPP'
#pragma once

#include <magic_enum/magic_enum.hpp>
#include <string_view>
#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/PacketIds.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

class Packet {
public:
    static constexpr PacketIds ID = PacketIds::Unknown;

public:
    virtual ~Packet() = default;
    virtual std::string_view getName() = 0;
    virtual PacketIds getId() = 0;

    virtual void write(BinaryStream& stream) = 0;
    virtual void read(ReadOnlyBinaryStream& stream) = 0;
};
CPP);
    }

    private function writeGeneratedRuntimeTypes(string $output, array $declarations): void
    {
        $byName = [];
        foreach ($declarations as $declaration) {
            $byName[(string) ($declaration['name'] ?? '')] = $declaration;
        }

        foreach (['PlayerAuthInputFlags', 'PlayerBlockAction'] as $name) {
            if (!isset($byName[$name])) {
                throw new RuntimeException('Source declaration is missing: ' . $name);
            }
        }
        foreach (['MetadataProperty', 'ItemStack', 'ItemStackWrapper', 'AbilitiesData', 'AbilitiesLayer', 'PropertySyncData'] as $name) {
            if (!isset($byName[$name])) {
                throw new RuntimeException('Source declaration is missing: ' . $name);
            }
        }

        $this->write(
            $this->runtimeTypePath($output, $byName['PlayerAuthInputFlags'], 'PlayerAuthInputFlags'),
            $this->playerAuthInputFlagsHeader($byName['PlayerAuthInputFlags']),
        );
        $this->write(
            $this->runtimeTypePath($output, $byName['PlayerBlockAction'], 'PlayerBlockAction'),
            $this->playerBlockActionHeader(
                $byName['PlayerBlockAction'],
                $byName['PlayerBlockActionWithBlockInfo'] ?? null,
                $byName['PlayerBlockActionStopBreak'] ?? null,
                $this->typeIncludePaths['PlayerAction'] ?? 'types/PlayerAction.hpp',
            ),
        );
        $this->write(
            $this->runtimeTypePath($output, $byName['MetadataProperty'], 'MetadataProperty'),
            $this->metadataPropertyHeader($byName),
        );
        $this->write(
            $this->runtimeTypePath($output, $byName['ItemStack'], 'NetworkItemDescriptor'),
            $this->networkItemDescriptorHeader($byName['ItemStack']),
        );
        $this->write(
            $this->runtimeTypePath($output, $byName['ItemStackWrapper'], 'NetworkItemStack'),
            $this->networkItemStackHeader(
                $byName['ItemStack'],
                $byName['ItemStackWrapper'],
                $this->typeIncludePaths['NetworkItemDescriptor'] ?? 'types/NetworkItemDescriptor.hpp',
            ),
        );
        $this->write(
            $this->runtimeTypePath($output, $byName['AbilitiesLayer'], 'AbilitiesLayer'),
            $this->abilitiesLayerHeader(),
        );
        $this->write(
            $this->runtimeTypePath($output, $byName['AbilitiesData'], 'AbilitiesData'),
            $this->abilitiesDataHeader($this->typeIncludePaths['AbilitiesLayer'] ?? 'types/AbilitiesLayer.hpp'),
        );
        $this->write(
            $this->runtimeTypePath($output, $byName['PropertySyncData'], 'PropertySyncData'),
            $this->propertySyncDataHeader(),
        );
    }

    private function propertySyncDataHeader(): string
    {
        return <<<'CPP'
#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

class PropertySyncData {
public:
    std::vector<std::pair<uint32_t, int32_t>> intProperties;
    std::vector<std::pair<uint32_t, float>> floatProperties;

    void read(ReadOnlyBinaryStream& stream) {
        intProperties.clear();
        const auto intCount = stream.readUnsignedVarInt().value_or(0);
        intProperties.reserve(intCount);
        for (uint32_t i = 0; i < intCount; ++i) {
            intProperties.emplace_back(
                stream.readUnsignedVarInt().value_or(0),
                stream.readVarInt().value_or(0)
            );
        }

        floatProperties.clear();
        const auto floatCount = stream.readUnsignedVarInt().value_or(0);
        floatProperties.reserve(floatCount);
        for (uint32_t i = 0; i < floatCount; ++i) {
            floatProperties.emplace_back(
                stream.readUnsignedVarInt().value_or(0),
                stream.readFloat().value_or(0.0f)
            );
        }
    }

    void write(BinaryStream& stream) const {
        stream.writeUnsignedVarInt(static_cast<uint32_t>(intProperties.size()));
        for (const auto& [key, value] : intProperties) {
            stream.writeUnsignedVarInt(key);
            stream.writeVarInt(value);
        }

        stream.writeUnsignedVarInt(static_cast<uint32_t>(floatProperties.size()));
        for (const auto& [key, value] : floatProperties) {
            stream.writeUnsignedVarInt(key);
            stream.writeFloat(value);
        }
    }
};
CPP;
    }

    private function buildPlatformHeader(): string
    {
        return <<<'CPP'
#pragma once

#include <cstdint>

enum class BuildPlatform : int32_t {
    Unknown = -1,
    Android = 1,
    IOS = 2,
    OSX = 3,
    Amazon = 4,
    GearVR = 5,
    HoloLens = 6,
    Windows10 = 7,
    Win32 = 8,
    Dedicated = 9,
    TVOS = 10,
    PlayStation = 11,
    Nintendo = 12,
    Xbox = 13,
    WindowsPhone = 14,
    Linux = 15,
};
CPP;
    }

    private function abilitiesLayerHeader(): string
    {
        return <<<'CPP'
#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

enum class AbilitiesLayerId : uint16_t {
    Cache = 0,
    Base = 1,
    Spectator = 2,
    Commands = 3,
    Editor = 4,
    LoadingScreen = 5,
};

class AbilitiesLayer {
public:
    AbilitiesLayerId layerId = AbilitiesLayerId::Base;
    uint32_t setAbilities = 0;
    uint32_t setAbilityValues = 0;
    float flySpeed = 0.0f;
    float verticalFlySpeed = 0.0f;
    float walkSpeed = 0.0f;

    void read(ReadOnlyBinaryStream& stream) {
        layerId = magic_enum::enum_cast<AbilitiesLayerId>(stream.readUnsignedShort().value_or(0)).value_or(AbilitiesLayerId::Cache);
        setAbilities = stream.readUnsignedInt().value_or(0);
        setAbilityValues = stream.readUnsignedInt().value_or(0);
        flySpeed = stream.readFloat().value_or(0.0f);
        verticalFlySpeed = stream.readFloat().value_or(0.0f);
        walkSpeed = stream.readFloat().value_or(0.0f);
    }

    void write(BinaryStream& stream) const {
        stream.writeUnsignedShort(magic_enum::enum_integer(layerId));
        stream.writeUnsignedInt(setAbilities);
        stream.writeUnsignedInt(setAbilityValues);
        stream.writeFloat(flySpeed);
        stream.writeFloat(verticalFlySpeed);
        stream.writeFloat(walkSpeed);
    }
};
CPP;
    }

    private function abilitiesDataHeader(string $abilitiesLayerInclude): string
    {
        $body = <<<'CPP'
#pragma once

#include <cstdint>
#include <vector>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <bedrock/protocol/@ABILITIES_LAYER_INCLUDE@>

class AbilitiesData {
public:
    int64_t targetActorUniqueId = 0;
    uint8_t playerPermission = 0;
    uint8_t commandPermission = 0;
    std::vector<AbilitiesLayer> layers;

    void read(ReadOnlyBinaryStream& stream) {
        targetActorUniqueId = stream.readLong().value_or(0);
        playerPermission = stream.readUnsignedByte().value_or(0);
        commandPermission = stream.readUnsignedByte().value_or(0);

        const auto layerCount = stream.readUnsignedByte().value_or(0);
        for (uint8_t i = 0; i < layerCount; ++i) {
            AbilitiesLayer layer;
            layer.read(stream);
            layers.push_back(layer);
        }
    }

    void write(BinaryStream& stream) const {
        stream.writeLong(targetActorUniqueId);
        stream.writeUnsignedByte(playerPermission);
        stream.writeUnsignedByte(commandPermission);
        stream.writeUnsignedByte(static_cast<uint8_t>(layers.size()));
        for (const auto& layer : layers) {
            layer.write(stream);
        }
    }
};
CPP;
        return str_replace('@ABILITIES_LAYER_INCLUDE@', $abilitiesLayerInclude, $body);
    }
}
