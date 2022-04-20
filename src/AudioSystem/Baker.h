#ifndef SYNTHIA_FIRMWARE_BAKER_H
#define SYNTHIA_FIRMWARE_BAKER_H

#include "../Model/SlotConfig.h"
#include <array>
#include <Util/Task.h>
#include <Loop/LoopListener.h>

class SlotBaker;
class EditSlot;

class Baker : public LoopListener {
public:
	Baker(const std::array<SlotConfig, 5>& configs);
	void start();
	bool isBaking();
	bool isDone();
	void loop(uint micros) override;

private:
	std::array<SlotConfig, 5> configs;
	SlotBaker* slotBakers[5];
	File slotFiles[5];
	EditSlot* editSlots[5];

	Task task;

	enum {
		WAITING, PREPARING, BAKING, DONE
	} state = WAITING;

	void prepare();
	void prepareSamples();
};


#endif //SYNTHIA_FIRMWARE_BAKER_H
