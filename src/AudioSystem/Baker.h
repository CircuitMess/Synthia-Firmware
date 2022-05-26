#ifndef SYNTHIA_FIRMWARE_BAKER_H
#define SYNTHIA_FIRMWARE_BAKER_H

#include "../Model/SlotConfig.h"
#include <array>
#include <Util/Task.h>
#include <Loop/LoopListener.h>
#include <functional>

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
	void setOneDoneCallback(const std::function<void(uint8_t)>& oneDoneCallback);

private:
	std::array<SlotConfig, 5> configs;
	std::array<File, 5> slotFiles;

	SlotBaker* baker = nullptr;
	EditSlot* editSlot = nullptr;

	uint8_t baked = 0;

	enum {
		WAITING, BAKING, DONE
	} state = WAITING;

	std::function<void(uint8_t)> oneDoneCallback;

};


#endif //SYNTHIA_FIRMWARE_BAKER_H
