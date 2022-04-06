#ifndef SYNTHIA_FIRMWARE_SLOTCONFIG_H
#define SYNTHIA_FIRMWARE_SLOTCONFIG_H

#include "Sample.hpp"
#include <SPIFFS.h>

struct SlotConfig {
	EffectData effects[(uint8_t)EffectData::Type::COUNT] = {
			{EffectData::Type::LOWPASS, 0},
			{EffectData::Type::HIGHPASS, 0},
			{EffectData::Type::REVERB, 0},
			{EffectData::Type::BITCRUSHER, 0},
			{EffectData::Type::VOLUME, 255}
	};
	Sample sample;
	uint8_t slotIndex = 0;
	uint8_t speed = 127;
};

File openSample(SlotConfig config, const char* mode = "r");

#endif //SYNTHIA_FIRMWARE_SLOTCONFIG_H
