#include "SlotManager.h"

SlotManager slotManager;

SlotConfig SlotManager::load(uint8_t slot){
	//TODO - load from SPIFFS
	return SlotConfig();
}

void SlotManager::store(uint8_t slot, SlotConfig config){
	//TODO - save to SPIFFS
}
