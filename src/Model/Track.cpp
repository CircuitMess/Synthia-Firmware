#include "Track.h"

#define TRACK_TIMELINE_COLS 16
#define TRACK_TIMELINE_ROWS 5

void TrackTimeline::set(uint8_t col, uint8_t row){
	if(col >= TRACK_TIMELINE_COLS || row >= TRACK_TIMELINE_ROWS) return;

	slotData[row] |= (1 << col);
}

void TrackTimeline::clear(uint8_t col){
	if(col >= TRACK_TIMELINE_COLS) return;

	uint16_t mask = ~(1 << col);
	for(auto &row:slotData){
		row &= mask;
	}
}

void TrackTimeline::clear(uint8_t col, uint8_t row){
	if(col >= TRACK_TIMELINE_COLS || row >= TRACK_TIMELINE_ROWS) return;

	slotData[row] &= ~(1 << col);
}

bool TrackTimeline::get(uint8_t col, uint8_t row){
	if(col >= TRACK_TIMELINE_COLS || row >= TRACK_TIMELINE_ROWS) return false;

	return slotData[row] & (1 << col);
}
