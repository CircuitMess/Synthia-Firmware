#ifndef SYNTHIA_FIRMWARE_TRACKVISUALIZER_H
#define SYNTHIA_FIRMWARE_TRACKVISUALIZER_H

#include "Visualizer.h"
#include "../Model/Track.h"

struct TrackVisData {
	TrackTimeline timeline;
	uint8_t cursor = 0;
};

class TrackVisualizer : public Visualizer<TrackVisData> {
protected:
	void visualize() override;
};


#endif //SYNTHIA_FIRMWARE_TRACKVISUALIZER_H
