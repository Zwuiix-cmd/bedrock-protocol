#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <optional>

#include <bedrock/protocol/types/sound/SoundDataEventType.hpp>
#include <bedrock/protocol/util/ProtocolUtil.hpp>

namespace bedrock::protocol {

class SoundDataEvent {
public:
    SoundDataEventType type = SoundDataEventType::Stop;
    std::optional<float> volume;
    std::optional<float> pitch;
    std::optional<float> duration;
    std::optional<float> targetVolume;
    std::optional<float> seconds;

    virtual ~SoundDataEvent() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        const auto rawType = stream.readUnsignedByte().value_or(0);
        const auto casted = magic_enum::enum_cast<SoundDataEventType>(rawType);
        if (!casted.has_value()) {
            stream.markOverflowed();
            return;
        }
        type = *casted;
        volume.reset();
        pitch.reset();
        duration.reset();
        targetVolume.reset();
        seconds.reset();

        switch (type) {
            case SoundDataEventType::Stop:
            case SoundDataEventType::Pause:
            case SoundDataEventType::Resume:
                break;
            case SoundDataEventType::SetVolume:
                volume = stream.readFloat().value_or(0.0f);
                break;
            case SoundDataEventType::SetPitch:
                pitch = stream.readFloat().value_or(0.0f);
                break;
            case SoundDataEventType::Fade:
                duration = stream.readFloat().value_or(0.0f);
                targetVolume = stream.readFloat().value_or(0.0f);
                break;
            case SoundDataEventType::SeekTo:
                seconds = stream.readFloat().value_or(0.0f);
                break;
        }
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedByte(util::narrow<uint8_t>(util::toUnderlying(type)));
        switch (type) {
            case SoundDataEventType::Stop:
            case SoundDataEventType::Pause:
            case SoundDataEventType::Resume:
                break;
            case SoundDataEventType::SetVolume:
                stream.writeFloat(volume.value_or(0.0f));
                break;
            case SoundDataEventType::SetPitch:
                stream.writeFloat(pitch.value_or(0.0f));
                break;
            case SoundDataEventType::Fade:
                stream.writeFloat(duration.value_or(0.0f));
                stream.writeFloat(targetVolume.value_or(0.0f));
                break;
            case SoundDataEventType::SeekTo:
                stream.writeFloat(seconds.value_or(0.0f));
                break;
        }
    }
};

} // namespace bedrock::protocol
