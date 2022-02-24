#ifndef SYNTHIA_FIRMWARE_EFFECTDATA_HPP
#define SYNTHIA_FIRMWARE_EFFECTDATA_HPP

#include <Arduino.h>
#include <Audio/EffectType.hpp>

struct EffectData {
	enum class Type : uint8_t {
		NONE, SPEED, LOWPASS, HIGHPASS, REVERB, BITCRUSHER, COUNT, SIZE
	} type = Type::NONE;
	uint8_t intensity = 0;
};

#endif //SYNTHIA_FIRMWARE_EFFECTDATA_HPP
