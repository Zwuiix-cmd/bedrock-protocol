#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <map>

namespace bedrock::protocol {


class GameRule {
public:
    bool isPlayerModifiable;

    GameRule() = default;
    virtual ~GameRule() = default;
    explicit GameRule(const bool& isPlayerModifiableValue) : isPlayerModifiable(isPlayerModifiableValue) {}

    virtual int32_t getTypeId() const = 0;
    virtual void write(BinaryStream& stream, const bool& isStartGame) const = 0;
    virtual void read(ReadOnlyBinaryStream& stream) = 0;

    static std::shared_ptr<GameRule> create(ReadOnlyBinaryStream& stream);

    static std::shared_ptr<GameRule> create(ReadOnlyBinaryStream& stream, const bool& isPlayerModifiable, const bool& isStartGame);
    static std::map<std::string, std::shared_ptr<GameRule>> readRules(ReadOnlyBinaryStream& stream, const bool& isStartGame);
    static void writeRules(BinaryStream& stream, const std::map<std::string, std::shared_ptr<GameRule>>& rules, const bool& isStartGame);
};

} // namespace bedrock::protocol
