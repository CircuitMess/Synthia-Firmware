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
		PLAY, SET, RELEASE, PAUSE
	} type;
	uint8_t slot;
	SampleSlot* sampleSlot;
};

class PlaybackSystem {
public:
	PlaybackSystem();
	void begin();
	void stop();

	void play(uint8_t slot);

	void set(uint8_t slot, File file, const SlotConfig& config);
	void edit(uint8_t slot, EditSlot* editSlot);
	void block(uint8_t slot);
	void release(uint8_t slot);
	void pause();
	void resume();
	uint8_t getVolume() const;
	void setVolume(uint8_t volume);

	SampleSlot* getSlot(uint8_t slot);

	const SlotConfig& getConfig(uint8_t slot);

	bool isPaused();
	bool isPlaying();

private:
	Mixer mixer;
	OutputI2S output;
	SampleSlot* slots[5];
	uint8_t volume = 0;

	Task task;
	Queue jobs;
	bool paused = false;

	static void taskFunc(Task* task);

	void processJob(AudioJob &job);

	SlotConfig configs[5];
};

extern PlaybackSystem Playback;


#endif //SYNTHIA_FIRMWARE_PLAYBACKSYSTEM_H
