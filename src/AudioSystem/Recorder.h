#ifndef SYNTHIA_FIRMWARE_RECORDER_H
#define SYNTHIA_FIRMWARE_RECORDER_H

#include <Arduino.h>
#include <FS.h>
#include <Loop/LoopListener.h>
#include <Audio/OutputI2S.h>
#include <Util/Task.h>
#include <Setup.hpp>

class Recorder{
public:
	Recorder();
	virtual ~Recorder();

	void start();
	void stop();
	void commit(File& file); //saves from RAM to SPIFFS

	bool isRecording();
	bool isRecorded();

	float getProgress();

private:
	void recordFunc();
	static void writeHeaderWAV(File& file, size_t size);

	enum {
		WAITING, RECORDING, DONE
	} state = WAITING;

	const float maxRecordTime = 1.0f; // in seconds
	// i2s buffer: int16 stereo
	// wav buffer: int16 mono
	const size_t i2sBufferSize = BUFFER_SAMPLES * 4; // 2 channels * 2 bytes per sample
	const size_t wavBufferSize = SAMPLE_RATE * BYTES_PER_SAMPLE * maxRecordTime;
	const size_t maxSamples = SAMPLE_RATE * maxRecordTime; // time * sampleRate * 4 bytes per sample (sample is int16_t, 2 channels)

	int16_t* wavBuffer;
	char* i2sBuffer;
	size_t sampleCount = 0;

	Task task;

};


#endif //SYNTHIA_FIRMWARE_RECORDER_H
