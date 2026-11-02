#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <utility>
#include <map>


class Experiments {
public:
    std::map<std::string, bool> experiments;
    bool hasPreviouslyUsedExperiments;

    Experiments() = default;
    explicit Experiments(const std::map<std::string, bool>& experimentsValue, const bool& hasPreviouslyUsedExperimentsValue) : experiments(experimentsValue), hasPreviouslyUsedExperiments(hasPreviouslyUsedExperimentsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        const auto experimentsCount = stream.readUnsignedInt().value_or(0);
        for (std::size_t index = 0; index < experimentsCount; ++index) {
            auto experimentsKey = stream.readString().value_or("");
            auto experimentsValue = stream.readBool().value_or(false);
            experiments.emplace(std::move(experimentsKey), std::move(experimentsValue));
        }
        hasPreviouslyUsedExperiments = stream.readBool().value_or(false);
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
