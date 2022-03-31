#ifndef SYNTHIA_FIRMWARE_BAKER_H
#define SYNTHIA_FIRMWARE_BAKER_H

#include "../Model/SlotConfig.h"
#include <array>
#include <Util/Task.h>

class SlotBaker;

class Baker {
public:
	Baker(std::array<SlotConfig, 5>& configs);
	void start();
	bool isDone();

private:
	std::array<SlotConfig, 5>& configs;
	std::array<SlotBaker*, 5> slotBakers;
	std::array<File, 5> slotFiles;
	Task task;

	enum {
		WAITING, WORKING, DONE
	} state = WAITING;

	void bake();
};


#endif //SYNTHIA_FIRMWARE_BAKER_H
