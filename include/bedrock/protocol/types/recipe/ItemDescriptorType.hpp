#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class ItemDescriptorType : uint16_t {
    Empty = 0,
    StringIdMeta = 1,
    Molang = 2,
    Tag = 3,
};

} // namespace bedrock::protocol
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::ItemDescriptorType>(::bedrock::protocol::ItemDescriptorType value) noexcept {
    switch (value) {
        case ::bedrock::protocol::ItemDescriptorType::Empty: return "empty";
        case ::bedrock::protocol::ItemDescriptorType::StringIdMeta: return "name";
        case ::bedrock::protocol::ItemDescriptorType::Molang: return "molang";
        case ::bedrock::protocol::ItemDescriptorType::Tag: return "item_tag";
        default: return invalid_tag;
    }
}

}
