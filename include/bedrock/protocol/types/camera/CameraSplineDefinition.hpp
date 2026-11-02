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
#include <bedrock/protocol/types/camera/CameraSplineInstruction.hpp>

class CameraSplineDefinition {
public:
    std::string name;
    std::shared_ptr<CameraSplineInstruction> instruction;

    CameraSplineDefinition() = default;
    explicit CameraSplineDefinition(const std::string& nameValue, const std::shared_ptr<CameraSplineInstruction>& instructionValue) : name(nameValue), instruction(instructionValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        instruction = std::make_shared<CameraSplineInstruction>();
        instruction->read(stream);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		instruction->write(stream);
    }
};
