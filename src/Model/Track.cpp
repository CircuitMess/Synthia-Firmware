#include "Track.h"

void TrackTimeline::set(uint8_t col, uint8_t row){
	slotData[col] |= (1 << row);
}

void TrackTimeline::clear(uint8_t col){
	uint16_t mask = ~(1 << col);
	for(auto &row:slotData){
		row &= mask;
	}
}

void TrackTimeline::clear(uint8_t col, uint8_t row){
	slotData[col] &= ~(1 << row);
}

bool TrackTimeline::get(uint8_t col, uint8_t row){
	return slotData[col] & (1 << row);
}
