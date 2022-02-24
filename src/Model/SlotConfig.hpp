#ifndef SYNTHIA_FIRMWARE_SLOTCONFIG_H
#define SYNTHIA_FIRMWARE_SLOTCONFIG_H

#include "EffectData.hpp"

struct SlotConfig {
	EffectData effects[(uint8_t)EffectData::Type::SIZE - 1];
	enum class SampleType : uint8_t {
		KICK, SNARE, CLAP, CLOSEDHIHAT, OPENHIHAT, RIMSHOT, BASS, RECORDING
	} sample = SampleType::KICK;
	uint8_t slotIndex = 0;
	uint8_t speed = 0;
};
#endif //SYNTHIA_FIRMWARE_SLOTCONFIG_H
