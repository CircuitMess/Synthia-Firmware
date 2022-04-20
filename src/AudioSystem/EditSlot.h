#ifndef SYNTHIA_FIRMWARE_EDITSLOT_H
#define SYNTHIA_FIRMWARE_EDITSLOT_H

#include "SampleSlot.h"
#include "../Model/SlotConfig.h"
#include "PlaybackSlot.h"
#include <Audio/EffectProcessor.h>
#include <Audio/SpeedModifier.h>
#include <Sync/Mutex.h>


class EditSlot : public SampleSlot {
public:
	EditSlot(const SlotConfig& config, File file);
	~EditSlot() override;

	void setEffect(EffectData::Type type, uint8_t intensity);
	void setSpeed(uint8_t speed);

	Generator& getGenerator() override;
	SlotConfig getConfig();

	void seek(size_t pos, SeekMode mode = SeekSet) override;

private:
	SlotConfig config;
	SpeedModifier speeder;
	EffectProcessor effector;

	Effect* effects[4];
	std::unique_ptr<PlaybackSlot> playback;

};


#endif //SYNTHIA_FIRMWARE_EDITSLOT_H
