#ifndef SYNTHIA_FIRMWARE_TRANSCODER_H
#define SYNTHIA_FIRMWARE_TRANSCODER_H

#include <Arduino.h>
#include <FS.h>
#include <Util/Task.h>
#include <Data/FileDataSource.h>
#include <Audio/Source.h>
#include <Audio/Output.h>

enum TranscoderType { WAV2AAC, AAC2WAV, CUSTOM };

class Transcoder {
public:
	typedef TranscoderType Type;

	/**
	 * Used for transcoding audio files. Transcoding is done on core 0, priority 1.
	 * @param type AAC2WAV, WAV2AAC, or CUSTOM. Custom requires setting audio source and output with setCoders
	 * @param in Input audio file
	 * @param out Output audio file
	 */
	Transcoder(Type type, File in, File out);
	virtual ~Transcoder();

	/**
	 * Set input decoder and output encoder. Only applies to CUSTOM type.
	 * @param source Audio source (SourceAAC / SourceWAV)
	 * @param output Audio output (OutputAAC / OutputWAV)
	 */
	void setCoders(Source* source, Output* output);

	void start();
	bool isDone();

private:
	Type type;

	File in;
	File out;

	FileDataSource dataSource;

	Source* src = nullptr;
	Output* dst = nullptr;

	Task task;
	enum { WAITING, WORKING, DONE } state = WAITING;

	void transcode();

};

#endif //SYNTHIA_FIRMWARE_TRANSCODER_H
