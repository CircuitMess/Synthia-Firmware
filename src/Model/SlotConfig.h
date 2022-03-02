#ifndef SYNTHIA_FIRMWARE_SLOTCONFIG_H
#define SYNTHIA_FIRMWARE_SLOTCONFIG_H

#include "Sample.hpp"
#include <SPIFFS.h>

struct SlotConfig {
	EffectData effects[(uint8_t)EffectData::Type::COUNT];
	Sample sample;
	uint8_t slotIndex = 0;
	uint8_t speed = 0;
};

File openSample(SlotConfig config);

#endif //SYNTHIA_FIRMWARE_SLOTCONFIG_H
