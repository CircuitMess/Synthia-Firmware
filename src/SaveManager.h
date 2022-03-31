#ifndef SYNTHIA_FIRMWARE_SAVEMANAGER_H
#define SYNTHIA_FIRMWARE_SAVEMANAGER_H

#include <Arduino.h>
#include "Model/SaveData.hpp"

class SaveManager {
public:
	SaveData load(uint8_t trackSlot);
	void store(uint8_t trackSlot, SaveData config);

private:
	void copyFile(File& source, File& destination);
	void clearFolder(File& folder);
	void copyFolder(File& source, File& destination);
};

extern SaveManager saveManager;

#endif //SYNTHIA_FIRMWARE_SAVEMANAGER_H
