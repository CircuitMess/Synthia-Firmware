#ifndef SYNTHIA_FIRMWARE_PLAYBACKSYSTEM_H
#define SYNTHIA_FIRMWARE_PLAYBACKSYSTEM_H

#include "EditSlot.h"
#include <Audio/Mixer.h>
#include <Audio/OutputI2S.h>
#include <Util/Task.h>
#include <Sync/Mutex.h>
#include <Sync/Queue.h>

struct AudioJob {
	enum {
		PLAY, SET
	} type;
	uint8_t slot;
	SampleSlot* sampleSlot;
};

class PlaybackSystem {
public:
	PlaybackSystem();
	void begin();

	void play(uint8_t slot);

	void set(uint8_t slot, File file);
	EditSlot* edit(uint8_t slot, const SlotConfig& config);
	void block(uint8_t slot);
	uint8_t getVolume() const;
	void setVolume(uint8_t volume);

	const SlotConfig& getConfig(uint8_t slot);
private:
	Mixer mixer;
	OutputI2S output;
	SampleSlot* slots[5];
	uint8_t volume = 100;

	Task task;
	Queue jobs;

	static void taskFunc(Task* task);

	void processJob(AudioJob &job);

	SlotConfig configs[5];
};

extern PlaybackSystem Playback;


#endif //SYNTHIA_FIRMWARE_PLAYBACKSYSTEM_H
