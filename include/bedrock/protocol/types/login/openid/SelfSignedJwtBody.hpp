#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

#include <bedrock/protocol/types/login/JwtBodyRfc7519.hpp>
namespace bedrock::protocol {


class SelfSignedJwtBody : public JwtBodyRfc7519 {
public:
    std::string cpk;
    std::string leguuid;
    std::string xname;
    std::string mid;
    int32_t ap;
    std::string nid;
    std::string nname;
    std::string pid;
    std::string pname;
    std::string xid;

    SelfSignedJwtBody() = default;
    virtual ~SelfSignedJwtBody() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        cpk = stream.readString().value_or("");
        leguuid = stream.readString().value_or("");
        xname = stream.readString().value_or("");
        mid = stream.readString().value_or("");
        ap = stream.readInt().value_or(0);
        nid = stream.readString().value_or("");
        nname = stream.readString().value_or("");
        pid = stream.readString().value_or("");
        pname = stream.readString().value_or("");
        xid = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(cpk);
        stream.writeString(leguuid);
        stream.writeString(xname);
        stream.writeString(mid);
        stream.writeInt(ap);
        stream.writeString(nid);
        stream.writeString(nname);
        stream.writeString(pid);
        stream.writeString(pname);
        stream.writeString(xid);
    }
};

} // namespace bedrock::protocol
