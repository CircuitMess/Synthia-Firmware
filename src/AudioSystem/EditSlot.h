#ifndef SYNTHIA_FIRMWARE_EDITSLOT_H
#define SYNTHIA_FIRMWARE_EDITSLOT_H

#include "SampleSlot.h"
#include "../Model/SlotConfig.h"
#include "PlaybackSlot.h"
#include <Audio/EffectProcessor.h>
#include <Audio/SpeedModifier.h>


class EditSlot : public SampleSlot {
public:
	EditSlot(SlotConfig config);
	~EditSlot() override;
	void setEffect(EffectData::Type type, uint8_t intensity);
	void setSpeed(uint8_t speed);
	Generator & getGenerator() override;
	void seek(size_t pos, SeekMode mode) override;
	SlotConfig getConfig();

private:
	SlotConfig config;
	File sampleFile;
	PlaybackSlot *playback;
	SpeedModifier speeder;
	EffectProcessor effector;

	Effect* effects[4];
};


#endif //SYNTHIA_FIRMWARE_EDITSLOT_H
