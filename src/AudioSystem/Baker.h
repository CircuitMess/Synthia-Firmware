#ifndef SYNTHIA_FIRMWARE_BAKER_H
#define SYNTHIA_FIRMWARE_BAKER_H

#include "../Model/SlotConfig.h"
#include <array>
#include <Util/Task.h>

class SlotBaker;
class EditSlot;

class Baker {
public:
	Baker(const std::array<SlotConfig, 5>& configs);
	void start();
	bool isDone();

private:
	std::array<SlotConfig, 5> configs;
	SlotBaker* slotBakers[5];
	File slotFiles[5];
	EditSlot* editSlots[5];

	Task task;

	enum {
		WAITING, WORKING, DONE
	} state = WAITING;

	void bake();
	void prepareSamples();
};


#endif //SYNTHIA_FIRMWARE_BAKER_H
