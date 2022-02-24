#ifndef SYNTHIA_FIRMWARE_EDITSLOT_H
#define SYNTHIA_FIRMWARE_EDITSLOT_H

#include "SampleSlot.h"
#include "../Model/SlotConfig.hpp"
#include "PlaybackSlot.h"
#include <Audio/EffectProcessor.h>
#include <Audio/SpeedModifier.h>


class EditSlot : public SampleSlot {
public:
	EditSlot(SlotConfig config);
	void setEffect(EffectData::Type type, uint8_t intensity);
	void setSpeed(uint8_t speed);

private:
	PlaybackSlot playback;
	EffectProcessor effector;
	SpeedModifier speeder;

	Effect *effects[4];

	static File openFile(SlotConfig config);
};


#endif //SYNTHIA_FIRMWARE_EDITSLOT_H
