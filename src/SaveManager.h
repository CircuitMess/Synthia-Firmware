#ifndef SYNTHIA_FIRMWARE_SAVEMANAGER_H
#define SYNTHIA_FIRMWARE_SAVEMANAGER_H

#include <Arduino.h>
#include <Loop/LoopListener.h>
#include <Devices/Matrix/MatrixPixel.h>
#include <Sync/Queue.h>
#include "Model/SaveData.hpp"

class SaveManager : public LoopListener {
public:
	SaveManager();
	void begin();

	SaveData load(uint8_t trackSlot, bool saveLastEdited = false);
	void store(uint8_t trackSlot, SaveData config,  bool saveLastEdited = false);
	SaveData loadLast();

	void loop(uint micros) override;

private:
	void copyFile(File& source, File& destination);
	void clearFolder(File& folder);
	void storeRecordings(File& source, File& destination);
	void loadRecordings(File& source, File& destination);
	void copyFolder(File& source, File& destination);
	void saveLast(uint8_t trackSlot);
	uint8_t getLast();

	static SaveData defaultData;

	struct LEDAction {
		enum { Set, Clear } action;
		uint8_t slot;
		MatrixPixel color;
	};
	Queue actions;
	void sendLEDAction(LEDAction action);
};

extern SaveManager saveManager;

#endif //SYNTHIA_FIRMWARE_SAVEMANAGER_H
