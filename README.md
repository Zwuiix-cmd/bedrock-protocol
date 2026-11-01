# Kodiak Bedrock Protocol

Local source repository for keeping a reproducible Bedrock protocol snapshot
and generating the C++ metadata that Kodiak can consume later.

The upstream PHP implementation is kept as a Git submodule at
`upstream/BedrockProtocol`. The parent repository owns the parser, snapshots,
and C++ generation; the upstream files are never modified by the generator.

## Bootstrap

```powershell
git submodule update --init --recursive
python tools/generate_protocol.py --strict
python -m unittest discover -s tests -v
```

The generator records the upstream commit, commit date, protocol versions,
packet IDs, packet declarations, and `PacketPool` registration order in
`generated/metadata/protocol.json`. It also emits preview-only C++ headers in
`generated/cpp/`:

- `PacketIds.hpp` contains the protocol ID enum and ID metadata.
- `PacketPool.hpp` contains the ordered registration descriptors.

These generated headers are deliberately not copied into the client yet. The
future integration step will map the generated model onto
`client/src/kodiak/sdk/custom/packet` and `client/src/kodiak/sdk/network`.

## Reproducibility

The source commit is part of every generated artifact. To check whether the
checked-in output matches the current submodule revision, run:

```powershell
python tools/generate_protocol.py --strict --check
```

The current upstream project is LGPL-3.0; its license and attribution remain
inside the submodule.
