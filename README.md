# bedrock-protocol

A C++20 library for the Minecraft Bedrock network protocol, generated from
the PHP definitions in [axolotl-pm/BedrockProtocol](https://github.com/axolotl-pm/BedrockProtocol).

## Build

The project provides a CMake library target. GLM, magic_enum, and EnTT for
`PacketPool` are fetched automatically.

```powershell
cmake -S . -B build
cmake --build build --config Release
```

Use it from another CMake project:

```cmake
add_subdirectory(path/to/bedrock-protocol)
target_link_libraries(MyApp PRIVATE bedrock-protocol::bedrock-protocol)
```

To build the protocol headers without `PacketPool` or EnTT:

```powershell
cmake -S . -B build -DBEDROCK_PROTOCOL_BUILD_POOL=OFF
```

When Kodiak already provides the dependencies, disable their fetch separately:

```powershell
cmake -S . -B build `
    -DBEDROCK_PROTOCOL_FETCH_ENTT=OFF `
    -DBEDROCK_PROTOCOL_FETCH_GLM=OFF `
    -DBEDROCK_PROTOCOL_FETCH_MAGIC_ENUM=OFF
```

## Using a packet

```cpp
#include <bedrock/protocol/SetTimePacket.hpp>
#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

BinaryStream encoded;
SetTimePacket packet;
packet.time = 42;
packet.write(encoded);

ReadOnlyBinaryStream input(encoded.getBuffer());
SetTimePacket decoded;
decoded.read(input);
```

`PacketPool` creates a packet from its `PacketIds` value:

```cpp
#include <bedrock/protocol/pool/PacketPool.hpp>

PacketPool::setup();
auto packet = PacketPool::createPacket(PacketIds::SetTime);
```

## Regenerating the protocol

Generated C++ files are already included in `include/`.

```powershell
composer install
composer generate
composer clean
```

The source version is configured in [`generator.json`](generator.json). The
generator produces packets, types, enums, `PacketIds`, and `PacketPool`.

## License

The original code in this repository is covered by [`LICENSE`](LICENSE). It
may be used as a library, but nobody may claim, relicense, sell, or
redistribute the original source as their own.

Protocol files derived from upstream remain subject to the upstream LGPL-3.0
license: [`LICENSE-UPSTREAM-LGPL-3.0.txt`](LICENSE-UPSTREAM-LGPL-3.0.txt).
