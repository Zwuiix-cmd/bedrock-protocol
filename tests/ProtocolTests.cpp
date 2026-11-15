#include <bedrock/protocol/CameraInstructionPacket.hpp>
#include <bedrock/protocol/CraftingDataPacket.hpp>
#include <bedrock/protocol/InventoryTransactionPacket.hpp>
#include <bedrock/protocol/LoginPacket.hpp>
#include <bedrock/protocol/MobEffectPacket.hpp>
#include <bedrock/protocol/PacketIds.hpp>
#include <bedrock/protocol/PlayerAuthInputPacket.hpp>
#include <bedrock/protocol/SetHudPacket.hpp>
#include <bedrock/protocol/UpdateBlockSyncedPacket.hpp>
#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <bedrock/protocol/pool/PacketPool.hpp>
#include <bedrock/protocol/types/MapInfoRequestPacketClientPixel.hpp>
#include <bedrock/protocol/types/MobEffectIds.hpp>
#include <bedrock/protocol/types/inventory/InventoryTransactionChangedSlotsHack.hpp>
#include <bedrock/protocol/types/inventory/MismatchTransactionData.hpp>
#include <bedrock/protocol/types/inventory/NormalTransactionData.hpp>
#include <bedrock/protocol/types/inventory/ReleaseItemTransactionData.hpp>
#include <bedrock/protocol/types/inventory/UseItemOnEntityTransactionData.hpp>
#include <bedrock/protocol/types/inventory/UseItemTransactionData.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequest.hpp>
#include <bedrock/protocol/types/inventory/stackresponse/ItemStackResponse.hpp>

#include "GeneratedPacketCoverage.hpp"

#include <magic_enum/magic_enum.hpp>

#include <cstdint>
#include <exception>
#include <iostream>
#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

using namespace bedrock::protocol;

namespace {

void expect(bool condition, std::string_view message) {
    if (!condition) {
        throw std::runtime_error(std::string(message));
    }
}

std::string encodeSignedVarInt(int32_t value) {
    BinaryStream stream;
    stream.writeVarInt(value);
    return stream.getBuffer();
}

std::string binaryString(std::initializer_list<uint8_t> bytes) {
    std::string result;
    result.reserve(bytes.size());
    for (const auto byte : bytes) {
        result.push_back(static_cast<char>(byte));
    }
    return result;
}

void testSignedVarInts() {
    const std::vector<std::pair<int32_t, std::string>> fixtures{
        {0, binaryString({0x00})},
        {1, binaryString({0x02})},
        {-1, binaryString({0x01})},
        {64, binaryString({0x80, 0x01})},
        {std::numeric_limits<int32_t>::max(), binaryString({0xfe, 0xff, 0xff, 0xff, 0x0f})},
        {std::numeric_limits<int32_t>::min(), binaryString({0xff, 0xff, 0xff, 0xff, 0x0f})},
    };

    for (const auto& [value, expectedBytes] : fixtures) {
        const auto encoded = encodeSignedVarInt(value);
        expect(encoded == expectedBytes, "signed VarInt bytes changed");

        ReadOnlyBinaryStream input(encoded);
        const auto decoded = input.readVarInt();
        expect(decoded.has_value() && decoded.value() == value, "signed VarInt roundtrip failed");
        expect(!input.hasOverflowed(), "signed VarInt unexpectedly overflowed");
    }
}

void testStringViewsAndLoginPacket() {
    const std::string stringValue = "connection-request";
    BinaryStream stringOutput;
    stringOutput.writeString(stringValue);

    ReadOnlyBinaryStream stringInput(stringOutput.getBuffer());
    const auto stringView = stringInput.readStringView();
    expect(stringView.has_value() && *stringView == stringValue, "string view did not decode");
    expect(stringInput.position() == stringInput.size(), "string view left unread bytes");

    const std::string byteArrayValue = "jwt-payload";
    ReadOnlyBinaryStream byteArrayInput(byteArrayValue);
    const auto byteArrayView = byteArrayInput.readByteArrayView(byteArrayValue.size());
    expect(byteArrayView.has_value() && *byteArrayView == byteArrayValue, "byte array view did not decode");
    expect(byteArrayInput.position() == byteArrayInput.size(), "byte array view left unread bytes");

    LoginPacket packet;
    packet.protocol = 924;
    packet.authInfoJson = "{\"DisplayName\":\"Kodiak\"}";
    packet.clientDataJwt = "header.payload.signature";

    BinaryStream output;
    packet.write(output);

    LoginPacket decoded;
    ReadOnlyBinaryStream input(output.getBuffer());
    decoded.read(input);
    expect(!input.hasOverflowed(), "valid Login packet overflowed");
    expect(input.position() == input.size(), "valid Login packet left unread bytes");
    expect(decoded.protocol == packet.protocol, "Login protocol changed");
    expect(decoded.authInfoJson == packet.authInfoJson, "Login auth info changed");
    expect(decoded.clientDataJwt == packet.clientDataJwt, "Login client data changed");
}

InventoryTransactionPacket makePacket(InventoryTransactionPacket::Type type) {
    InventoryTransactionPacket packet;
    packet.requestId = 0;

    switch (type) {
        case InventoryTransactionPacket::Type::Normal:
            packet.trData = std::make_shared<NormalTransactionData>();
            break;
        case InventoryTransactionPacket::Type::Mismatch:
            packet.trData = std::make_shared<MismatchTransactionData>();
            break;
        case InventoryTransactionPacket::Type::UseItem: {
            auto data = std::make_shared<UseItemTransactionData>();
            data->actionType = UseItemTransactionData::Action::ClickBlock;
            data->triggerType = TriggerType::PlayerInput;
            data->blockPosition = glm::ivec3(0);
            data->face = 0;
            data->hotbarSlot = 0;
            data->itemInHand = NetworkItemStack{};
            data->playerPosition = glm::vec3(0.0f);
            data->clickPosition = glm::vec3(0.0f);
            data->blockRuntimeId = 0;
            data->clientInteractPrediction = PredictedResult::Failure;
            data->clientCooldownState = 0;
            packet.trData = std::move(data);
            break;
        }
        case InventoryTransactionPacket::Type::UseItemOnEntity: {
            auto data = std::make_shared<UseItemOnEntityTransactionData>();
            data->actorRuntimeId = 0;
            data->actionType = UseItemOnEntityTransactionData::Action::Interact;
            data->hotbarSlot = 0;
            data->itemInHand = NetworkItemStack{};
            data->playerPosition = glm::vec3(0.0f);
            data->clickPosition = glm::vec3(0.0f);
            packet.trData = std::move(data);
            break;
        }
        case InventoryTransactionPacket::Type::ReleaseItem: {
            auto data = std::make_shared<ReleaseItemTransactionData>();
            data->actionType = ReleaseItemTransactionData::Action::Release;
            data->hotbarSlot = 0;
            data->itemInHand = NetworkItemStack{};
            data->headPosition = glm::vec3(0.0f);
            packet.trData = std::move(data);
            break;
        }
    }

    return packet;
}

void testInventoryTransactionTypes() {
    for (const auto type : magic_enum::enum_values<InventoryTransactionPacket::Type>()) {
        auto packet = makePacket(type);
        BinaryStream output;
        packet.write(output);

        InventoryTransactionPacket decoded;
        ReadOnlyBinaryStream input(output.getBuffer());
        decoded.read(input);

        expect(!input.hasOverflowed(), "valid inventory transaction overflowed");
        expect(input.position() == input.size(), "valid inventory transaction left unread bytes");
        expect(decoded.trData != nullptr, "inventory transaction data was not created");
        expect(decoded.trData->getTypeId() == magic_enum::enum_integer(type), "wrong transaction data type");
    }

    auto packet = makePacket(InventoryTransactionPacket::Type::Mismatch);
    packet.requestId = -1;
    auto changedSlots = std::make_shared<InventoryTransactionChangedSlotsHack>();
    changedSlots->containerId = 5;
    changedSlots->changedSlotIndexes = {1, 200};
    packet.requestChangedSlots.emplace().push_back(changedSlots);

    BinaryStream output;
    packet.write(output);
    const std::string expected{"\x01\x01\x01\x05\x02\x01\xc8\x01\x00", 9};
    expect(output.getBuffer() == expected, "inventory request ID did not use signed VarInt encoding");

    InventoryTransactionPacket decoded;
    ReadOnlyBinaryStream input(output.getBuffer());
    decoded.read(input);
    expect(decoded.requestId == -1, "signed inventory request ID did not roundtrip");
    expect(decoded.requestChangedSlots && decoded.requestChangedSlots->size() == 1, "changed slot list size changed");
    expect(decoded.requestChangedSlots->front()->changedSlotIndexes == std::vector<uint8_t>({1, 200}), "changed slot values changed");
}

void testStackRequestAndResponseIds() {
    ItemStackRequest request;
    request.requestId = -1;
    request.filterStringCause = 0;

    BinaryStream requestOutput;
    request.write(requestOutput);
    ReadOnlyBinaryStream requestInput(requestOutput.getBuffer());
    ItemStackRequest decodedRequest;
    decodedRequest.read(requestInput);
    expect(decodedRequest.requestId == -1, "item stack request ID did not roundtrip");
    expect(!requestInput.hasOverflowed(), "item stack request overflowed");

    ItemStackResponse response;
    response.result = ItemStackResponse::Result::Error;
    response.requestId = -1;

    BinaryStream responseOutput;
    response.write(responseOutput);
    ReadOnlyBinaryStream responseInput(responseOutput.getBuffer());
    ItemStackResponse decodedResponse;
    decodedResponse.read(responseInput);
    expect(decodedResponse.result == ItemStackResponse::Result::Error, "item stack response result changed");
    expect(decodedResponse.requestId == -1, "item stack response ID did not roundtrip");
    expect(!decodedResponse.containerInfos.has_value(), "error item stack response read container data");
    expect(!responseInput.hasOverflowed(), "item stack response overflowed");
}

void testPlayerAuthInputBlockActions() {
    PlayerAuthInputPacket packet;
    packet.position = glm::vec3(0.0f);
    packet.pitch = 0.0f;
    packet.yaw = 0.0f;
    packet.headYaw = 0.0f;
    packet.moveVecX = 0.0f;
    packet.moveVecZ = 0.0f;
    packet.inputFlags.addFlag(InputData::PerformBlockActions);
    packet.inputMode = InputMode::MouseKeyboard;
    packet.playMode = PlayMode::Normal;
    packet.interactionMode = InteractionMode::Touch;
    packet.interactRotation = glm::vec2(0.0f);
    packet.tick = 0;
    packet.delta = glm::vec3(0.0f);
    packet.blockActions.emplace();
    packet.blockActions->push_back(PlayerBlockAction{});
    packet.analogMoveVecX = 0.0f;
    packet.analogMoveVecZ = 0.0f;
    packet.cameraOrientation = glm::vec3(0.0f);
    packet.rawMove = glm::vec2(0.0f);

    BinaryStream output;
    packet.write(output);

    PlayerAuthInputPacket decoded;
    ReadOnlyBinaryStream input(output.getBuffer());
    decoded.read(input);
    expect(!input.hasOverflowed(), "PlayerAuthInput with block actions overflowed");
    expect(decoded.blockActions.has_value(), "empty-or-nonempty block action optional was disengaged");
    expect(decoded.blockActions->size() == 1, "block action count changed");
    expect(decoded.blockActions->front().actionType == PlayerAction::StopBreak, "block action type changed");

    packet.blockActions->clear();
    output.reset();
    packet.write(output);
    PlayerAuthInputPacket emptyDecoded;
    ReadOnlyBinaryStream emptyInput(output.getBuffer());
    emptyDecoded.read(emptyInput);
    expect(!emptyInput.hasOverflowed(), "empty PlayerAuthInput block action list overflowed");
    expect(emptyDecoded.blockActions.has_value() && emptyDecoded.blockActions->empty(), "empty block action list did not engage optional");
}

void testOptionalPacketFields() {
    CameraInstructionPacket packet;
    BinaryStream output;
    packet.write(output);

    CameraInstructionPacket decoded;
    ReadOnlyBinaryStream input(output.getBuffer());
    decoded.read(input);
    expect(!input.hasOverflowed(), "optional camera packet overflowed");
    expect(!decoded.set.has_value(), "absent optional camera field became engaged");
    expect(!decoded.fade.has_value(), "absent optional camera field became engaged");
    expect(!decoded.target.has_value(), "absent optional camera field became engaged");
}

void testMobEffectIdType() {
    static_assert(std::is_same_v<decltype(MobEffectPacket{}.effectId), MobEffectIds>);

    MobEffectPacket packet;
    packet.actorRuntimeId = 42;
    packet.eventId = MobEffectPacket::Event::Add;
    packet.effectId = MobEffectIds::Nausea;
    packet.amplifier = 1;
    packet.particles = true;
    packet.duration = 200;
    packet.tick = 10;
    packet.ambient = false;

    BinaryStream output;
    packet.write(output);

    MobEffectPacket decoded;
    ReadOnlyBinaryStream input(output.getBuffer());
    decoded.read(input);
    expect(decoded.effectId == MobEffectIds::Nausea, "MobEffect effect ID enum changed");
    expect(!input.hasOverflowed(), "MobEffect packet overflowed");
    expect(input.position() == input.size(), "MobEffect packet left unread bytes");
}

void testGeneratedCodecFixes() {
    UpdateBlockSyncedPacket synced;
    synced.blockPosition = {1, -2, 3};
    synced.blockRuntimeId = 7;
    synced.flags = static_cast<UpdateBlockPacket::Flag>(3);
    synced.actorUniqueId = 300;
    synced.updateType = UpdateBlockSyncedPacket::Type::Create;

    BinaryStream syncedOutput;
    synced.write(syncedOutput);
    const std::string expectedSynced{"\x02\x03\x06\x07\x03\x00\xac\x02\x01", 9};
    expect(syncedOutput.getBuffer() == expectedSynced, "UpdateBlockSynced used the wrong VarLong codec");

    UpdateBlockSyncedPacket decodedSynced;
    ReadOnlyBinaryStream syncedInput(syncedOutput.getBuffer());
    decodedSynced.read(syncedInput);
    expect(decodedSynced.blockPosition == synced.blockPosition, "UpdateBlockSynced lost its block position");
    expect(decodedSynced.flags == synced.flags, "UpdateBlockSynced changed combined block flags");
    expect(decodedSynced.actorUniqueId == synced.actorUniqueId, "UpdateBlockSynced actor ID changed");
    expect(decodedSynced.updateType == synced.updateType, "UpdateBlockSynced update type changed");

    SetHudPacket hud;
    hud.hudElements = {HudElement::PaperDoll, HudElement::Armor};
    hud.visibility = HudVisibility::Hide;

    BinaryStream hudOutput;
    hud.write(hudOutput);
    SetHudPacket decodedHud;
    ReadOnlyBinaryStream hudInput(hudOutput.getBuffer());
    decodedHud.read(hudInput);
    expect(decodedHud.hudElements == hud.hudElements, "SetHud enum list changed");
    expect(decodedHud.visibility == hud.visibility, "SetHud visibility changed");

    BinaryStream pixelOutput;
    pixelOutput.writeUnsignedInt(0x44332211);
    pixelOutput.writeUnsignedShort(5 + (2 * MapInfoRequestPacketClientPixel::Y_INDEX_MULTIPLIER));
    MapInfoRequestPacketClientPixel pixel;
    ReadOnlyBinaryStream pixelInput(pixelOutput.getBuffer());
    pixel.read(pixelInput);
    expect(pixel.color == 0x44332211, "map pixel color changed");
    expect(pixel.x == 5 && pixel.y == 2, "map pixel coordinates changed");

    BinaryStream pixelRoundtrip;
    pixel.write(pixelRoundtrip);
    expect(pixelRoundtrip.getBuffer() == pixelOutput.getBuffer(), "map pixel codec did not roundtrip");
}

void testMalformedBuffers() {
    const auto valid = [&]() {
        auto packet = makePacket(InventoryTransactionPacket::Type::UseItem);
        BinaryStream output;
        packet.write(output);
        return output.getBuffer();
    }();

    for (std::size_t length = 0; length < valid.size(); ++length) {
        ReadOnlyBinaryStream input(std::string_view(valid).substr(0, length));
        InventoryTransactionPacket packet;
        try {
            packet.read(input);
        } catch (const std::exception&) {
        } catch (...) {
        }
    }

    const std::string unknownType{"\x00\x00\x63", 3};
    ReadOnlyBinaryStream unknownInput(unknownType);
    InventoryTransactionPacket unknownPacket;
    bool rejected = false;
    try {
        unknownPacket.read(unknownInput);
    } catch (...) {
        rejected = true;
    }
    expect(rejected, "unknown inventory transaction type was accepted");

    auto withTrailingData = valid + std::string{"\x7f", 1};
    ReadOnlyBinaryStream trailingInput(withTrailingData);
    InventoryTransactionPacket trailingPacket;
    trailingPacket.read(trailingInput);
    expect(!trailingInput.hasOverflowed(), "valid packet with trailing data overflowed");
    expect(trailingInput.position() < trailingInput.size(), "trailing data was unexpectedly consumed");
}

void testMalformedCollectionBounds() {
    const std::string oversizedCount{"\xff\xff\xff\x7f", 4};
    ReadOnlyBinaryStream input(oversizedCount);
    const auto values = input.readList([&]() {
        return input.readUnsignedByte().value_or(0);
    });
    expect(values.empty(), "malformed collection unexpectedly decoded entries");
    expect(input.hasOverflowed(), "malformed collection did not stop at the stream boundary");
}

void testPacketPool() {
    generated_packet_tests::run(expect);
    expect(PacketPool::createPacket(PacketIds::Unknown) == nullptr, "PacketPool accepted Unknown packet ID");
    expect(PacketPool::createPacket<InventoryTransactionPacket>() != nullptr, "PacketPool type lookup failed");
    expect(PacketPool::createPacket(PacketIds::CraftingData) != nullptr, "CraftingData was not registered");
    expect(PacketPool::createPacket<CraftingDataPacket>() != nullptr, "CraftingData type was not registered");
    PacketPool::unregisterPacket(PacketIds::CraftingData);
    expect(PacketPool::createPacket(PacketIds::CraftingData) == nullptr, "CraftingData ID remained registered");
    expect(PacketPool::createPacket<CraftingDataPacket>() == nullptr, "CraftingData type remained registered");
    PacketPool::unregisterPacket(PacketIds::CraftingData);
    PacketPool::unregisterPacket(PacketIds::Unknown);
    PacketPool::setup();
    expect(PacketPool::createPacket<CraftingDataPacket>() == nullptr, "setup restored an unregistered packet");
    expect(PacketPool::createPacket<InventoryTransactionPacket>() != nullptr, "unregister removed another packet type");
    expect(PacketPool::createPacket(PacketIds::InventoryTransaction) != nullptr, "unregister removed another packet ID");
}

} // namespace

int main() {
    try {
        testSignedVarInts();
        testStringViewsAndLoginPacket();
        testInventoryTransactionTypes();
        testStackRequestAndResponseIds();
        testPlayerAuthInputBlockActions();
        testOptionalPacketFields();
        testMobEffectIdType();
        testGeneratedCodecFixes();
        testMalformedBuffers();
        testMalformedCollectionBounds();
        testPacketPool();
    } catch (const std::exception& exception) {
        std::cerr << "protocol tests failed: " << exception.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "protocol tests failed with an unknown exception\n";
        return 1;
    }

    std::cout << "protocol tests passed\n";
    return 0;
}
