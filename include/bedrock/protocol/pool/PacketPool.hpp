#pragma once

#include <entt/entt.hpp>
#include <unordered_map>
#include <shared_mutex>
#include <mutex>
#include <memory>
#include <type_traits>
#include <utility>
#include <functional>
#include <cassert>
#include <bedrock/protocol/Packet.hpp>

namespace bedrock::protocol {

class PacketPool {
public:
    using Constructor = std::shared_ptr<Packet>(*)();

    static void setup() noexcept;

    static inline void unregisterPacket(PacketIds id) noexcept {
        std::unique_lock lock(s_mutex);
        const auto it = s_byId.find(id);
        if (it == s_byId.end()) return;
        const Constructor constructor = it->second;
        s_byId.erase(it);
        std::erase_if(s_byType, [constructor](const auto& entry) {
            return entry.second == constructor;
        });
    }

    static inline std::shared_ptr<Packet> createPacket(PacketIds id) noexcept {
        std::shared_lock lock(s_mutex);
        auto it = s_byId.find(id);
        if (it == s_byId.end()) return nullptr;
        Constructor ctor = it->second;
        assert(ctor != nullptr);
        return ctor();
    }

    template<typename P>
    requires std::derived_from<P, Packet>
    static inline std::shared_ptr<P> createPacket() noexcept {
        const entt::id_type tid = entt::type_hash<P>::value();
        std::shared_lock lock(s_mutex);
        auto itType = s_byType.find(tid);
        if (itType == s_byType.end()) return nullptr;

        Constructor ctor = itType->second;
        if (!ctor) return nullptr;

        std::shared_ptr<Packet> base = ctor();
        if (!base) return nullptr;
        return std::static_pointer_cast<P>(base);
    }

private:
    template<typename P>
    requires std::derived_from<P, Packet>
    static void registerPacket() noexcept;

    template<typename P>
    static std::shared_ptr<Packet> ctor() {
        return std::static_pointer_cast<Packet>(std::make_shared<P>());
    }

private:
    static inline std::unordered_map<PacketIds, Constructor> s_byId;
    static inline std::unordered_map<entt::id_type, Constructor> s_byType;
    static inline std::shared_mutex s_mutex;
    static inline std::once_flag s_setupFlag;
    static constexpr size_t ExpectedPacketCount = 239;
};

} // namespace bedrock::protocol