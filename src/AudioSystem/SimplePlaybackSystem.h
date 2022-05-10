#ifndef SYNTHIA_FIRMWARE_SIMPLEPLAYBACKSYSTEM_H
#define SYNTHIA_FIRMWARE_SIMPLEPLAYBACKSYSTEM_H

#include "EditSlot.h"
#include <Audio/Mixer.h>
#include <Audio/OutputI2S.h>
#include <Util/Task.h>
#include <Sync/Mutex.h>
#include <Sync/Queue.h>
#include <Audio/SourceAAC.h>
#include "PlaybackSystem.h"

class SimplePlaybackSystem {
public:
	SimplePlaybackSystem();
	virtual ~SimplePlaybackSystem();

	void begin();

	void play(File file);
	void stop();

	void setVolume(uint8_t volume);

	bool isPlaying();

private:
	OutputI2S output;

	FileDataSource* dataSource = nullptr;
	SourceAAC* source = nullptr;

	Task task;

	struct PlayJob {
		FileDataSource* dataSource;
		SourceAAC* source;
	};
	Queue playQueue;

	void player();

	bool running = false;

};

#endif //SYNTHIA_FIRMWARE_SIMPLEPLAYBACKSYSTEM_H
