#ifndef SYNTHIA_FIRMWARE_WAV2AAC_H
#define SYNTHIA_FIRMWARE_WAV2AAC_H

#include <Arduino.h>
#include <FS.h>
#include <Audio/SourceWAV.h>
#include <Audio/OutputAAC.h>
#include <Util/Task.h>
#include <Data/FileDataSource.h>

class WAV2AAC {
public:
	WAV2AAC(File in, File out);

	void start();
	bool isDone();

private:
	File in;
	File out;

	FileDataSource dataSource;

	SourceWAV src;
	OutputAAC dst;

	Task task;
	enum { WAITING, WORKING, DONE } state = WAITING;

	void transcode();

};


#endif //SYNTHIA_FIRMWARE_WAV2AAC_H
