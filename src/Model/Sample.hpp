#ifndef SYNTHIA_FIRMWARE_SAMPLE_HPP
#define SYNTHIA_FIRMWARE_SAMPLE_HPP

#include "EffectData.hpp"

struct Sample{
	enum class Type : uint8_t {
		KICK, SNARE, CLAP, CLOSEDHIHAT, OPENHIHAT, RECORDING, SIZE
	} type;
	uint8_t fileIndex;
};
#endif //SYNTHIA_FIRMWARE_SAMPLE_HPP
