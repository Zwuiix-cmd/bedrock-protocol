#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>

#include <bedrock/protocol/types/DataStoreValue.hpp>
#include <bedrock/protocol/types/BoolDataStoreValue.hpp>
#include <bedrock/protocol/types/DoubleDataStoreValue.hpp>
#include <bedrock/protocol/types/StringDataStoreValue.hpp>
#include <bedrock/protocol/types/DataStore.hpp>

class DataStoreUpdate : public DataStore {
public:
    static constexpr int64_t ID = 0;

    std::string name;
    std::string property;
    std::string path;
    std::shared_ptr<DataStoreValue> data;
    uint32_t updateCount;
    uint32_t pathUpdateCount;

    DataStoreUpdate() = default;
    virtual ~DataStoreUpdate() = default;
    explicit DataStoreUpdate(
        const std::string& nameValue,
        const std::string& propertyValue,
        const std::string& pathValue,
        const std::shared_ptr<DataStoreValue>& dataValue,
        const uint32_t& updateCountValue,
        const uint32_t& pathUpdateCountValue
    ) :
        name(nameValue),
        property(propertyValue),
        path(pathValue),
        data(dataValue),
        updateCount(updateCountValue),
        pathUpdateCount(pathUpdateCountValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        name = stream.readString().value_or("");
        property = stream.readString().value_or("");
        path = stream.readString().value_or("");
        data = DataStoreValue::create(stream);
        updateCount = stream.readUnsignedInt().value_or(0);
        pathUpdateCount = stream.readUnsignedInt().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(name);
		stream.writeString(property);
		stream.writeString(path);
		stream.writeUnsignedVarInt(data->getTypeId());
		data->write(stream);
		stream.writeUnsignedInt(updateCount);
		stream.writeUnsignedInt(pathUpdateCount);
    }
};
