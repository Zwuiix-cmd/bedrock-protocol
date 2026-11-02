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
#include <bedrock/protocol/types/sound/SoundDataEventType.hpp>
#include <bedrock/protocol/types/sound/SoundDataEventType.hpp>
#include <bedrock/protocol/types/sound/SoundDataEventType.hpp>

class SoundDataEvent {
public:
    SoundDataEventType type;
    std::optional<float> volume;
    std::optional<float> pitch;
    std::optional<float> duration;
    std::optional<float> targetVolume;
    std::optional<float> seconds;

    SoundDataEvent() = default;
    explicit SoundDataEvent(
        const SoundDataEventType& typeValue,
        const std::optional<float>& volumeValue,
        const std::optional<float>& pitchValue,
        const std::optional<float>& durationValue,
        const std::optional<float>& targetVolumeValue,
        const std::optional<float>& secondsValue
    ) :
        type(typeValue),
        volume(volumeValue),
        pitch(pitchValue),
        duration(durationValue),
        targetVolume(targetVolumeValue),
        seconds(secondsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        type = static_cast<SoundDataEventType>(stream.readVarInt().value_or(0));
        volume = stream.readOptional([&]() {
			return stream.readFloat().value_or(0.0f);
		});
        pitch = stream.readOptional([&]() {
			return stream.readFloat().value_or(0.0f);
		});
        duration = stream.readOptional([&]() {
			return stream.readFloat().value_or(0.0f);
		});
        targetVolume = stream.readOptional([&]() {
			return stream.readFloat().value_or(0.0f);
		});
        seconds = stream.readOptional([&]() {
			return stream.readFloat().value_or(0.0f);
		});
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(static_cast<std::underlying_type_t<SoundDataEventType>>(type));
		stream.writeUnsignedByte(static_cast<std::underlying_type_t<SoundDataEventType>>(type));
		switch (type) {
			case SoundDataEventType::Stop:
			case SoundDataEventType::Pause:
			case SoundDataEventType::Resume: {
				break;
			}
			case SoundDataEventType::SetVolume: {
				if ((!volume)) {
					throw std::exception{};
				}
				stream.writeFloat(*(volume));
				break;
			}
			case SoundDataEventType::SetPitch: {
				if ((!pitch)) {
					throw std::exception{};
				}
				stream.writeFloat(*(pitch));
				break;
			}
			case SoundDataEventType::Fade: {
				if (((!duration) || (!targetVolume))) {
					throw std::exception{};
				}
				stream.writeFloat(*(duration));
				stream.writeFloat(*(targetVolume));
				break;
			}
			case SoundDataEventType::SeekTo: {
				if ((!seconds)) {
					throw std::exception{};
				}
				stream.writeFloat(*(seconds));
				break;
			}
		}
    }
};
