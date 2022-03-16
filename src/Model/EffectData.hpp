#ifndef SYNTHIA_FIRMWARE_EFFECTDATA_HPP
#define SYNTHIA_FIRMWARE_EFFECTDATA_HPP

#include <Arduino.h>

struct EffectData {
	enum class Type : uint8_t {
		LOWPASS, HIGHPASS, REVERB, BITCRUSHER, VOLUME, COUNT
	} type;
	uint8_t intensity;
};

#endif //SYNTHIA_FIRMWARE_EFFECTDATA_HPP
