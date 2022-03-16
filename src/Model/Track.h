#ifndef SYNTHIA_FIRMWARE_TRACK_H
#define SYNTHIA_FIRMWARE_TRACK_H

#include <Arduino.h>

struct TrackTimeline {
	uint16_t slotData[5] = {0};
	void set(uint8_t col, uint8_t row);
	void clear(uint8_t col);
	void clear(uint8_t col, uint8_t row);
	bool get(uint8_t col, uint8_t row);
};

struct Track{
	TrackTimeline timeline;
	uint8_t tempo = 127;
};
#endif //SYNTHIA_FIRMWARE_TRACK_H
