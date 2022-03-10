#ifndef SYNTHIA_FIRMWARE_SAMPLESLOT_H
#define SYNTHIA_FIRMWARE_SAMPLESLOT_H

#include <Audio/Source.h>

class SampleSlot {
public:
	virtual ~SampleSlot(){};

	void play(){
		seek(0, fs::SeekSet);
	}
	virtual void seek(size_t pos, SeekMode mode = SeekSet) = 0;
	virtual Generator & getGenerator() = 0;
};


#endif //SYNTHIA_FIRMWARE_SAMPLESLOT_H
