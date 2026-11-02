#pragma once

#include <cstdint>
#include <limits>
#include <magic_enum/magic_enum.hpp>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <bedrock/protocol/Forward.hpp>
#include <bedrock/protocol/types/camera/CameraAimAssistPresetExclusionDefinition.hpp>
#include <bedrock/protocol/types/camera/CameraAimAssistPresetItemSettings.hpp>
#include <bedrock/protocol/types/camera/CameraAimAssistPresetExclusionDefinition.hpp>
#include <bedrock/protocol/types/camera/CameraAimAssistPresetItemSettings.hpp>
#include <bedrock/protocol/types/camera/CameraAimAssistPresetExclusionDefinition.hpp>
#include <bedrock/protocol/types/camera/CameraAimAssistPresetItemSettings.hpp>

class CameraAimAssistPreset {
public:
    std::string identifier;
    std::shared_ptr<CameraAimAssistPresetExclusionDefinition> exclusionSettings;
    std::vector<std::string> liquidTargetingList;
    std::vector<std::shared_ptr<CameraAimAssistPresetItemSettings>> itemSettings;
    std::optional<std::string> defaultItemSettings;
    std::optional<std::string> defaultHandSettings;

    CameraAimAssistPreset() = default;
    explicit CameraAimAssistPreset(
        const std::string& identifierValue,
        const std::shared_ptr<CameraAimAssistPresetExclusionDefinition>& exclusionSettingsValue,
        const std::vector<std::string>& liquidTargetingListValue,
        const std::vector<std::shared_ptr<CameraAimAssistPresetItemSettings>>& itemSettingsValue,
        const std::optional<std::string>& defaultItemSettingsValue,
        const std::optional<std::string>& defaultHandSettingsValue
    ) :
        identifier(identifierValue),
        exclusionSettings(exclusionSettingsValue),
        liquidTargetingList(liquidTargetingListValue),
        itemSettings(itemSettingsValue),
        defaultItemSettings(defaultItemSettingsValue),
        defaultHandSettings(defaultHandSettingsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        identifier = stream.readString().value_or("");
        exclusionSettings = ([&]() {
    auto value = std::make_shared<CameraAimAssistPresetExclusionDefinition>();
    value->read(stream);
    return value;
}());
        liquidTargetingList = stream.readList([&]() {
			return stream.readString().value_or("");
		});
        itemSettings = stream.readList([&]() {
			return ([&]() {
			auto value = std::make_shared<CameraAimAssistPresetItemSettings>();
			value->read(stream);
			return value;
			}());
		});
        defaultItemSettings = stream.readOptional([&]() {
			return stream.readString().value_or("");
		});
        defaultHandSettings = stream.readOptional([&]() {
			return stream.readString().value_or("");
		});
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(identifier);
		exclusionSettings->write(stream);
		stream.writeUnsignedVarInt(liquidTargetingList.size());
		for (const auto& liquidTargeting : liquidTargetingList) {
			stream.writeString(liquidTargeting);
		}
		stream.writeUnsignedVarInt(itemSettings.size());
		for (const auto& itemSetting : itemSettings) {
			itemSetting->write(stream);
		}
		stream.writeOptional(defaultItemSettings, [&](const auto& value) {
					stream.writeString(value);
				});
		stream.writeOptional(defaultHandSettings, [&](const auto& value) {
					stream.writeString(value);
				});
    }
};
