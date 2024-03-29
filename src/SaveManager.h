#ifndef SYNTHIA_FIRMWARE_SAVEMANAGER_H
#define SYNTHIA_FIRMWARE_SAVEMANAGER_H

#include <Arduino.h>
#include "Model/SaveData.hpp"

class SaveManager {
public:
	SaveManager();
	void begin();

	SaveData load(uint8_t trackSlot, bool saveLastEdited = false);
	void store(uint8_t trackSlot, SaveData config,  bool saveLastEdited = false);
	SaveData loadLast();

private:
	void copyFile(File& source, File& destination);
	void clearFolder(File& folder);
	void storeRecordings(File& source, File& destination);
	void loadRecordings(File& source, File& destination);
	void copyFolder(File& source, File& destination);
	void saveLast(uint8_t trackSlot);
	uint8_t getLast();

	static SaveData defaultData;
};

extern SaveManager saveManager;

#endif //SYNTHIA_FIRMWARE_SAVEMANAGER_H
