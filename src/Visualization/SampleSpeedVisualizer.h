#ifndef SYNTHIA_FIRMWARE_SAMPLESPEEDVISUALIZER_H
#define SYNTHIA_FIRMWARE_SAMPLESPEEDVISUALIZER_H

#include "Visualizer.h"

class SampleSpeedVisualizer : public Visualizer<uint8_t> {
protected:
	void visualize() override;
	void onStop() override;
};


#endif //SYNTHIA_FIRMWARE_SAMPLESPEEDVISUALIZER_H
