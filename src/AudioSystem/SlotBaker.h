#ifndef SYNTHIA_FIRMWARE_SLOTBAKER_H
#define SYNTHIA_FIRMWARE_SLOTBAKER_H

#include "EditSlot.h"
#include <Util/Task.h>
#include <Audio/OutputAAC.h>

class SlotBaker {
public:
	SlotBaker(EditSlot* slot, File output);

	void start();
	bool isDone();

private:
	EditSlot* slot;
	File outFile;

	OutputAAC output;

	Task task;
	enum { WAITING, WORKING, DONE } state = WAITING;

	void bake();

};

#endif //SYNTHIA_FIRMWARE_SLOTBAKER_H
