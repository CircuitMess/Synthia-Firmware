#ifndef SYNTHIA_FIRMWARE_RECORDER_H
#define SYNTHIA_FIRMWARE_RECORDER_H

#include <Arduino.h>
#include <FS.h>
#include <Loop/LoopListener.h>
#include <Audio/OutputI2S.h>
#include <Util/Task.h>

class Recorder{
public:
	Recorder(uint8_t slot);

	void start();
	void stop();
	bool isRecorded();
	bool isRecording();

private:
	void recordingTask();
	void commit(); //saves from RAM to SPIFFS
	void writeHeaderWAV(size_t size);

	uint8_t* buf = nullptr;
	size_t cursor = 0;
	uint8_t slot = 0;
	bool recording = false;
	bool done = false;

	const uint8_t wavHeaderSize = 44;
	const uint32_t i2sBufferSize = 1600;
	const uint32_t sampleRate = 24000;
	const float maxRecordTime = 1.0f; // in seconds
	const uint wavFileSize = maxRecordTime * (float) sampleRate * 2.0f;
	const uint32_t wavBufferSize = sizeof(int16_t) * i2sBufferSize / 4; // i2sBuffer is stereo by byte, wavBuffer is mono int16
	const uint32_t numReadings = (maxRecordTime * sampleRate * 4) / i2sBufferSize; // time * sampleRate * 4 bytes per sample (sample is int16_t, 2 channels)

	int16_t* wavBuffer;
	char* i2sBuffer;
	uint32_t wavTotalWritten = 0;
	uint32_t recCounter = 0;

	OutputI2S i2s;
	Task task;

};


#endif //SYNTHIA_FIRMWARE_RECORDER_H
