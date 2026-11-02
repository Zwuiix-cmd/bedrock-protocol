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
#include <map>

class GameRule {
public:
    bool isPlayerModifiable;

    GameRule() = default;
    virtual ~GameRule() = default;
    explicit GameRule(const bool& isPlayerModifiableValue) : isPlayerModifiable(isPlayerModifiableValue) {}

    virtual int32_t getTypeId() const = 0;
    virtual void write(BinaryStream& stream, const bool& isStartGame) const = 0;
    virtual void read(ReadOnlyBinaryStream& stream) = 0;

    static std::shared_ptr<GameRule> decode(ReadOnlyBinaryStream& stream);

    static std::shared_ptr<GameRule> decode(ReadOnlyBinaryStream& stream, const bool& isPlayerModifiable, const bool& isStartGame);
    static std::map<std::string, std::shared_ptr<GameRule>> readRules(ReadOnlyBinaryStream& stream, const bool& isStartGame);
    static void writeRules(BinaryStream& stream, const std::map<std::string, std::shared_ptr<GameRule>>& rules, const bool& isStartGame);
};
