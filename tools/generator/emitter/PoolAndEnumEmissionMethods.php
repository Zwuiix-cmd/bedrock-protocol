<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\emitter;

use kodiak\bedrockprotocol\generator\Configuration;
use kodiak\bedrockprotocol\generator\CPPName;
use kodiak\bedrockprotocol\generator\CPP\CPPDefaultValueResolver;
use RuntimeException;

trait PoolAndEnumEmissionMethods
{
    private function writePacketPool(string $output, array $registrations): void
    {
        $base = $output . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock' . DIRECTORY_SEPARATOR . 'protocol';
        $poolBase = $base . DIRECTORY_SEPARATOR . 'pool';
        $this->makeDirectory($poolBase);
        $body = <<<'CPP'
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

class PacketPool {
public:
    using Constructor = std::shared_ptr<Packet>(*)();

    static void setup() noexcept;

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
        const entt::id_type tid = entt::type_id<P>();
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
    static constexpr size_t ExpectedPacketCount = 64;
};
CPP;
        $body = str_replace(
            'static constexpr size_t ExpectedPacketCount = 64;',
            'static constexpr size_t ExpectedPacketCount = ' . count($registrations) . ';',
            $body,
        );
        $this->write($poolBase . DIRECTORY_SEPARATOR . 'PacketPool.hpp', $body);

        $cpp = '#include <bedrock/protocol/pool/PacketPool.hpp>' . "\n\n";
        foreach ($registrations as $registration) {
            $cpp .= '#include <bedrock/protocol/' . CPPName::type($registration) . ".hpp>\n";
        }
        $cpp .= <<<'CPP'

void PacketPool::setup() noexcept {
    std::call_once(s_setupFlag, [](){
        {
            std::unique_lock lock(s_mutex);
            s_byId.reserve(ExpectedPacketCount);
            s_byType.reserve(ExpectedPacketCount);
        }

CPP;
        foreach ($registrations as $registration) {
            $cpp .= '        registerPacket<' . CPPName::type($registration) . '>();' . "\n";
        }
        $cpp .= <<<'CPP'
    });
}

template<typename P>
requires std::derived_from<P, Packet>
void PacketPool::registerPacket() noexcept {
    const entt::id_type tid = entt::type_hash<P>::value();

    std::unique_lock lock(s_mutex);

    if (s_byType.find(tid) != s_byType.end()) return;

    s_byType.emplace(tid, &PacketPool::ctor<P>);
    s_byId.emplace(P::ID, &PacketPool::ctor<P>);
}
CPP;
        $this->write($poolBase . DIRECTORY_SEPARATOR . 'PacketPool.cpp', $cpp);
    }

    private function isEnumDeclaration(array $declaration): bool
    {
        if (($declaration['kind'] ?? null) === 'enum') {
            return true;
        }
        if (($declaration['kind'] ?? null) !== 'class' || ($declaration['properties'] ?? []) !== []) {
            return false;
        }
        foreach ($declaration['methods'] ?? [] as $method) {
            if (($method['name'] ?? '') !== '__construct') {
                return false;
            }
        }
        $constants = array_values(array_filter(
            $declaration['constants'] ?? [],
            static fn (array $constant): bool => ($constant['name'] ?? '') !== 'NETWORK_ID',
        ));
        if ($constants === []) {
            return false;
        }
        foreach ($constants as $constant) {
            if (!is_int($constant['value'] ?? null) && !is_bool($constant['value'] ?? null)) {
                return false;
            }
        }
        return true;
    }

    private function enumCases(array $declaration): array
    {
        if (($declaration['kind'] ?? null) === 'enum') {
            return array_values(array_map(
                static fn (array $case): array => [
                    'name' => (string) ($case['name'] ?? ''),
                    'value' => $case['value'] ?? null,
                ],
                $declaration['cases'] ?? [],
            ));
        }
        return array_values(array_map(
            static fn (array $constant): array => [
                'name' => (string) ($constant['name'] ?? ''),
                'value' => is_bool($constant['value'] ?? null) ? (int) $constant['value'] : ($constant['value'] ?? null),
            ],
            array_values(array_filter(
                $declaration['constants'] ?? [],
                static fn (array $constant): bool => ($constant['name'] ?? '') !== 'NETWORK_ID',
            )),
        ));
    }

    private function enumUnderlyingType(array $declaration): string
    {
        if (($declaration['backingType'] ?? null) === 'string') {
            return 'uint16_t';
        }
        $values = array_values(array_filter(
            array_map(static fn (array $case): mixed => $case['value'] ?? null, $this->enumCases($declaration)),
            static fn (mixed $value): bool => is_int($value) || is_float($value),
        ));
        if ($values === []) {
            return 'uint16_t';
        }
        $minimum = min($values);
        $maximum = max($values);
        if ($minimum < 0) {
            return 'int32_t';
        }
        return $maximum <= 0xff ? 'uint8_t' : 'uint32_t';
    }

    private function isGeneratedType(array $declaration): bool
    {
        if (($declaration['kind'] ?? null) === 'enum') {
            return true;
        }
        if (!in_array($declaration['kind'] ?? null, ['class', 'interface'], true)) {
            return false;
        }
        if (in_array((string) $declaration['name'], ['AbilitiesData', 'AbilitiesLayer', 'PropertySyncData'], true)) {
            return false;
        }
        if ($this->catalog->isRuntimeDeclaration((string) $declaration['name'])) {
            return false;
        }
        return str_contains($declaration['namespace'], '\\types') || str_starts_with($declaration['file'], 'src/types/');
    }

    private function method(array $declaration, string $name): ?array
    {
        foreach ($declaration['methods'] as $method) {
            if ($method['name'] === $name) {
                return $method;
            }
        }
        $alias = match ($name) {
            'read' => 'decode',
            'write' => 'encode',
            default => null,
        };
        if ($alias !== null) {
            foreach ($declaration['methods'] as $method) {
                if ($method['name'] === $alias) {
                    return $method;
                }
            }
        }
        return null;
    }

    private function defaultValue(string $text): string
    {
        $text = preg_replace('/\btrue\b/i', 'true', $text) ?? $text;
        $text = preg_replace('/\bfalse\b/i', 'false', $text) ?? $text;
        $text = preg_replace('/\bnull\b/i', 'nullptr', $text) ?? $text;
        return $text;
    }

    private function methodReferencedTypes(array $declaration, array $types): array
    {
        $names = [];
        $visit = function (mixed $node) use (&$visit, &$names, $declaration): void {
            if (!$node instanceof \PhpParser\Node) {
                return;
            }
            if (($node instanceof \PhpParser\Node\Expr\StaticCall || $node instanceof \PhpParser\Node\Expr\New_)
                && $node->class instanceof \PhpParser\Node\Name) {
                $names[] = CPPName::type($node->class->getLast());
            }
            foreach ($node->getSubNodeNames() as $name) {
                $child = $node->{$name};
                if ($child instanceof \PhpParser\Node) {
                    $visit($child);
                } elseif (is_array($child)) {
                    foreach ($child as $item) {
                        if ($item instanceof \PhpParser\Node) {
                            $visit($item);
                        }
                    }
                }
            }
        };
        foreach ($declaration['methods'] ?? [] as $method) {
            foreach ($method['body'] ?? [] as $statement) {
                $visit($statement);
            }
        }
        $result = [];
        foreach ($types as $type) {
            if (in_array($this->generatedTypeName($type), $names, true)) {
                $result[] = $type;
            }
        }
        return $result;
    }

    private function methodReferencedIncludes(array $declaration): array
    {
        $names = [];
        $visit = function (mixed $node) use (&$visit, &$names, $declaration): void {
            if (!$node instanceof \PhpParser\Node) {
                return;
            }
            if (($node instanceof \PhpParser\Node\Expr\StaticCall || $node instanceof \PhpParser\Node\Expr\New_)
                && $node->class instanceof \PhpParser\Node\Name) {
                $className = $node->class->getLast();
                if ($className === 'self' || $className === 'static') {
                    $className = (string) ($declaration['name'] ?? $className);
                }
                $candidateNames = [$className];
                if ($node instanceof \PhpParser\Node\Expr\StaticCall
                    && $node->name instanceof \PhpParser\Node\Identifier) {
                    $alias = $this->catalog->staticCallAlias($className, $node->name->toString());
                    if (is_string($alias) && preg_match('/^([A-Za-z_][A-Za-z0-9_]*)::/', $alias, $matches) === 1) {
                        $candidateNames[] = $matches[1];
                    }
                }
                foreach ($candidateNames as $candidateName) {
                    $include = $this->catalog->includeFor($candidateName);
                    if ($include !== null) {
                        $names[$include] = true;
                    }
                }
            }
            foreach ($node->getSubNodeNames() as $name) {
                $child = $node->{$name};
                if ($child instanceof \PhpParser\Node) {
                    $visit($child);
                } elseif (is_array($child)) {
                    foreach ($child as $item) {
                        if ($item instanceof \PhpParser\Node) {
                            $visit($item);
                        }
                    }
                }
            }
        };
        foreach ($declaration['methods'] ?? [] as $method) {
            foreach ($method['body'] ?? [] as $statement) {
                $visit($statement);
            }
        }
        return array_keys($names);
    }
}
