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

class AttributeModifier {
public:
    std::string id;
    std::string name;
    float amount;
    int32_t operation;
    int32_t operand;
    bool serializable;

    AttributeModifier() = default;
    explicit AttributeModifier(
        const std::string& idValue,
        const std::string& nameValue,
        const float& amountValue,
        const int32_t& operationValue,
        const int32_t& operandValue,
        const bool& serializableValue
    ) :
        id(idValue),
        name(nameValue),
        amount(amountValue),
        operation(operationValue),
        operand(operandValue),
        serializable(serializableValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        id = stream.readString().value_or("");
        name = stream.readString().value_or("");
        amount = stream.readFloat().value_or(0.0f);
        operation = stream.readInt().value_or(0);
        operand = stream.readInt().value_or(0);
        serializable = stream.readBool().value_or(false);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(id);
		stream.writeString(name);
		stream.writeFloat(amount);
		stream.writeInt(operation);
		stream.writeInt(operand);
		stream.writeBool(serializable);
    }
};
