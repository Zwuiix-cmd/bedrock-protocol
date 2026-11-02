#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <string>

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
