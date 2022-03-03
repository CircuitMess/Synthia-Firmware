#ifndef SYNTHIA_FIRMWARE_PLAYBACKSYSTEM_H
#define SYNTHIA_FIRMWARE_PLAYBACKSYSTEM_H

#include <FS/RamFile.h>
#include "EditSlot.h"
#include <Loop/LoopListener.h>
#include <queue>
#include <Audio/Mixer.h>
#include <Audio/OutputI2S.h>
#include <Sync/Mutex.h>
#include <Sync/Queue.h>

struct AudioJob {
	enum {
		PLAY, SET
	} type;
	uint8_t slot;
	SampleSlot* sampleSlot;
};

class PlaybackSystem : public LoopListener {
public:
	PlaybackSystem();
	void init();
	void play(uint8_t slot);
	void set(uint8_t slot, File file);
	EditSlot* edit(uint8_t slot, SlotConfig config);
	void block(uint8_t slot);
	void loop(uint micros) override;

private:
	Mixer mixer;
	OutputI2S output;

	SampleSlot* slots[5];
	Queue jobs;
	void processJob(AudioJob &job);
};


#endif //SYNTHIA_FIRMWARE_PLAYBACKSYSTEM_H
