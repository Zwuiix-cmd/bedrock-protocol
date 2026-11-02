#pragma once

#include <cstdint>
#include <limits>
#include <magic_enum/magic_enum.hpp>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <bedrock/protocol/Forward.hpp>
#include <map>

class Experiments {
public:
    std::map<std::string, bool> experiments;
    bool hasPreviouslyUsedExperiments;

    Experiments() = default;
    explicit Experiments(const std::map<std::string, bool>& experimentsValue, const bool& hasPreviouslyUsedExperimentsValue) : experiments(experimentsValue), hasPreviouslyUsedExperiments(hasPreviouslyUsedExperimentsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        hasPreviouslyUsedExperiments = stream.readBool().value_or(false);
        experiments = ([&]() {
            std::map<std::string, bool> result;
            const auto count = stream.readUnsignedInt().value_or(0);
            for (uint32_t i = 0; i < count; ++i) {
                auto key = stream.readString().value_or("");
                auto value = stream.readBool().value_or(false);
                result.emplace(std::move(key), std::move(value));
            }
            return result;
        }());
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedInt(experiments.size());
		for (const auto& [experimentName, enabled] : experiments) {
			stream.writeString(experimentName);
			stream.writeBool(enabled);
		}
		stream.writeBool(hasPreviouslyUsedExperiments);
    }
};
