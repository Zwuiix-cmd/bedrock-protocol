#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

namespace bedrock::protocol {


class GatheringJoinInfo {
public:
    std::string experienceId;
    std::string experienceName;
    std::string experienceWorldId;
    std::string experienceWorldName;
    std::string creatorId;
    std::string storeId;

    GatheringJoinInfo() = default;
    explicit GatheringJoinInfo(
        const std::string& experienceIdValue,
        const std::string& experienceNameValue,
        const std::string& experienceWorldIdValue,
        const std::string& experienceWorldNameValue,
        const std::string& creatorIdValue,
        const std::string& storeIdValue
    ) :
        experienceId(experienceIdValue),
        experienceName(experienceNameValue),
        experienceWorldId(experienceWorldIdValue),
        experienceWorldName(experienceWorldNameValue),
        creatorId(creatorIdValue),
        storeId(storeIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        experienceId = stream.readString().value_or("");
        experienceName = stream.readString().value_or("");
        experienceWorldId = stream.readString().value_or("");
        experienceWorldName = stream.readString().value_or("");
        creatorId = stream.readString().value_or("");
        storeId = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(experienceId);
		stream.writeString(experienceName);
		stream.writeString(experienceWorldId);
		stream.writeString(experienceWorldName);
		stream.writeString(creatorId);
		stream.writeString(storeId);
    }
};

} // namespace bedrock::protocol
