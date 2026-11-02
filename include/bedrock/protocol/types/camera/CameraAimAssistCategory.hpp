#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <string>

#include <bedrock/protocol/types/camera/CameraAimAssistCategoryPriorities.hpp>
namespace bedrock::protocol {


class CameraAimAssistCategory {
public:
    std::string name;
    std::shared_ptr<CameraAimAssistCategoryPriorities> priorities;

    CameraAimAssistCategory() = default;
    explicit CameraAimAssistCategory(
        const std::string& nameValue,
        const std::shared_ptr<CameraAimAssistCategoryPriorities>& prioritiesValue
    ) :
        name(nameValue),
        priorities(prioritiesValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        priorities = std::make_shared<CameraAimAssistCategoryPriorities>();
        priorities->read(stream);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		priorities->write(stream);
    }
};

} // namespace bedrock::protocol
