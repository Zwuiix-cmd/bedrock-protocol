#include <bedrock/protocol/PacketIds.hpp>
#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <bedrock/protocol/pool/PacketPool.hpp>

#include <cstdint>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

using namespace bedrock::protocol;

namespace {

std::string fromHex(std::string_view hex) {
    auto nibble = [](char c) -> uint8_t {
        if (c >= '0' && c <= '9') return static_cast<uint8_t>(c - '0');
        if (c >= 'a' && c <= 'f') return static_cast<uint8_t>(c - 'a' + 10);
        if (c >= 'A' && c <= 'F') return static_cast<uint8_t>(c - 'A' + 10);
        return 0;
    };
    std::string out;
    out.reserve(hex.size() / 2);
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) {
        out.push_back(static_cast<char>((nibble(hex[i]) << 4) | nibble(hex[i + 1])));
    }
    return out;
}

std::string toHex(std::string_view bytes) {
    static constexpr char kHex[] = "0123456789abcdef";
    std::string out;
    out.reserve(bytes.size() * 2);
    for (unsigned char c : bytes) {
        out.push_back(kHex[(c >> 4) & 0xF]);
        out.push_back(kHex[c & 0xF]);
    }
    return out;
}

} // namespace

int main() {
    PacketPool::setup();

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        int32_t id = 0;
        std::string hex;
        if (!(iss >> id)) {
            std::cout << "ERROR invalid_input\n";
            continue;
        }
        iss >> hex; // optional for 0-byte packets

        auto packet = PacketPool::createPacket(static_cast<PacketIds>(id));
        if (!packet) {
            std::cout << "UNSUPPORTED " << id << "\n";
            continue;
        }

        try {
            const auto raw = fromHex(hex);
            ReadOnlyBinaryStream input(raw);
            packet->read(input);
            if (input.hasOverflowed()) {
                std::cout << "OVERFLOW\n";
                continue;
            }
            BinaryStream output;
            packet->write(output);
            std::cout << "OK " << toHex(output.getBuffer()) << "\n";
        } catch (const std::exception& e) {
            std::cout << "EXCEPTION " << e.what() << "\n";
        } catch (...) {
            std::cout << "EXCEPTION unknown\n";
        }
    }
    return 0;
}
