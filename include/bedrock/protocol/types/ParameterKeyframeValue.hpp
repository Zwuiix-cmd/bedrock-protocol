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

class ParameterKeyframeValue {
public:
    float time;
    glm::vec3 value;

    ParameterKeyframeValue() = default;
    explicit ParameterKeyframeValue(const float& timeValue, const glm::vec3& valueValue) : time(timeValue), value(valueValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        time = stream.readFloat().value_or(0.0f);
        value = stream.readVec3().value_or(glm::vec3(0.0f));
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeFloat(time);
		stream.writeVec3(value);
    }
};
