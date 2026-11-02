#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

#include <bedrock/protocol/types/login/JwtBodyRfc7519.hpp>

class XboxAuthJwtBody : public JwtBodyRfc7519 {
public:
    std::string ipt;
    std::string pfcd;
    std::string tid;
    std::string mid;
    std::string xid;
    std::string xname;
    std::string cpk;
    std::string pid;
    std::string pname;

    XboxAuthJwtBody() = default;
    virtual ~XboxAuthJwtBody() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        ipt = stream.readString().value_or("");
        pfcd = stream.readString().value_or("");
        tid = stream.readString().value_or("");
        mid = stream.readString().value_or("");
        xid = stream.readString().value_or("");
        xname = stream.readString().value_or("");
        cpk = stream.readString().value_or("");
        pid = stream.readString().value_or("");
        pname = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(ipt);
        stream.writeString(pfcd);
        stream.writeString(tid);
        stream.writeString(mid);
        stream.writeString(xid);
        stream.writeString(xname);
        stream.writeString(cpk);
        stream.writeString(pid);
        stream.writeString(pname);
    }
};
