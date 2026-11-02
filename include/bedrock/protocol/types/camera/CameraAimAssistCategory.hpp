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
#include <bedrock/protocol/types/camera/CameraAimAssistCategoryPriorities.hpp>

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
