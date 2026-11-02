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
#include <bedrock/protocol/types/shape/PrimitiveShapePayload.hpp>

class PrimitiveShapeLinePayload : public PrimitiveShapePayload {
public:
    static constexpr int64_t ID = 4;

    glm::vec3 lineEndLocation;

    PrimitiveShapeLinePayload() = default;
    virtual ~PrimitiveShapeLinePayload() = default;
    explicit PrimitiveShapeLinePayload(const glm::vec3& lineEndLocationValue) : lineEndLocation(lineEndLocationValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        lineEndLocation = stream.readVec3().value_or(glm::vec3(0.0f));
    }

    void write(BinaryStream& stream) const override {
		stream.writeVec3(lineEndLocation);
    }
};
