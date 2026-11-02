#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

#include <bedrock/protocol/types/DataStore.hpp>
namespace bedrock::protocol {


class DataStoreRemoval : public DataStore {
public:
    static constexpr int64_t ID = 2;

    std::string name;

    DataStoreRemoval() = default;
    virtual ~DataStoreRemoval() = default;
    explicit DataStoreRemoval(const std::string& nameValue) : name(nameValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        name = stream.readString().value_or("");
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(name);
    }
};

} // namespace bedrock::protocol
