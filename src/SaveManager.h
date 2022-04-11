#ifndef SYNTHIA_FIRMWARE_SAVEMANAGER_H
#define SYNTHIA_FIRMWARE_SAVEMANAGER_H

#include <Arduino.h>
#include "Model/SaveData.hpp"

class SaveManager {
public:
	SaveData load(uint8_t trackSlot, bool saveLastEdited = false);
	void store(uint8_t trackSlot, SaveData config,  bool saveLastEdited = false);
	SaveData loadLast();

private:
	void copyFile(File& source, File& destination);
	void clearFolder(File& folder);
	void copyFolder(File& source, File& destination);
	void saveLast(uint8_t trackSlot);
	uint8_t getLast();
};

extern SaveManager saveManager;

#endif //SYNTHIA_FIRMWARE_SAVEMANAGER_H
