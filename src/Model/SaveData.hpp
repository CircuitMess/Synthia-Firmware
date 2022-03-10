#ifndef SYNTHIA_FIRMWARE_SAVEDATA_HPP
#define SYNTHIA_FIRMWARE_SAVEDATA_HPP
#include "Track.h"
#include "SlotConfig.h"

struct SaveData{
	Track track;
	SlotConfig slots[5];
};
#endif //SYNTHIA_FIRMWARE_SAVEDATA_HPP
