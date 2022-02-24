#ifndef SYNTHIA_FIRMWARE_SAMPLESLOT_H
#define SYNTHIA_FIRMWARE_SAMPLESLOT_H

#include <Audio/Source.h>

class SampleSlot {
public:
	void play(){
		getSource().seek(0);
	}
	virtual Source& getSource() = 0;
};


#endif //SYNTHIA_FIRMWARE_SAMPLESLOT_H
