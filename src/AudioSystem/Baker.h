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
	std::array<File, 5> getFiles();
	std::array<SlotConfig, 5> getConfigs();

private:
	std::array<SlotConfig, 5> configs;
	SlotBaker* slotBakers[5] = { nullptr };
	std::array<File, 5> slotFiles;
	EditSlot* editSlots[5] = { nullptr };
	uint8_t baked = 0;

	Task task;

	enum {
		WAITING, PREPARING, BAKING, DONE
	} state = WAITING;

	void prepare();
	void prepareSamples();
	void cleanup();
};


#endif //SYNTHIA_FIRMWARE_BAKER_H
