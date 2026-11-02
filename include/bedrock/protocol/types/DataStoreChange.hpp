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
namespace bedrock::protocol {


class DataStoreChange : public DataStore {
public:
    static constexpr int64_t ID = 1;

    std::string name;
    std::string property;
    uint32_t updateCount;
    std::shared_ptr<DataStoreValue> data;

    DataStoreChange() = default;
    virtual ~DataStoreChange() = default;
    explicit DataStoreChange(
        const std::string& nameValue,
        const std::string& propertyValue,
        const uint32_t& updateCountValue,
        const std::shared_ptr<DataStoreValue>& dataValue
    ) :
        name(nameValue),
        property(propertyValue),
        updateCount(updateCountValue),
        data(dataValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        name = stream.readString().value_or("");
        property = stream.readString().value_or("");
        updateCount = stream.readUnsignedVarInt().value_or(0);
        data = DataStoreValue::create(stream);
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(name);
		stream.writeString(property);
		stream.writeUnsignedVarInt(updateCount);
		stream.writeUnsignedVarInt(data->getTypeId());
		data->write(stream);
    }
};

} // namespace bedrock::protocol
