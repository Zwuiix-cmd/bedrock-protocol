<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
#include <bedrock/protocol/Packet.hpp>
BEFORE,
<<<'AFTER'
#include <bedrock/protocol/UpdateBlockPacket.hpp>
AFTER],
    [<<<'BEFORE'
class UpdateBlockSyncedPacket : public Packet
BEFORE,
<<<'AFTER'
class UpdateBlockSyncedPacket : public UpdateBlockPacket
AFTER],
    [<<<'BEFORE'
void read(ReadOnlyBinaryStream& stream) override {
BEFORE,
<<<'AFTER'
void read(ReadOnlyBinaryStream& stream) override {
        UpdateBlockPacket::read(stream);
AFTER],
    [<<<'BEFORE'
void write(BinaryStream& stream) override {
BEFORE,
<<<'AFTER'
void write(BinaryStream& stream) override {
        UpdateBlockPacket::write(stream);
AFTER],
];
