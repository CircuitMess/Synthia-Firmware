#ifndef SYNTHIA_FIRMWARE_SLOTMANAGER_H
#define SYNTHIA_FIRMWARE_SLOTMANAGER_H

#include <Arduino.h>
#include "Model/SlotConfig.h"

class SlotManager {
public:
	SlotConfig load(uint8_t slot);
	void store(uint8_t slot, SlotConfig config);
};

extern SlotManager slotManager;

#endif //SYNTHIA_FIRMWARE_SLOTMANAGER_H
