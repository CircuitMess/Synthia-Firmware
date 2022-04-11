#ifndef SYNTHIA_FIRMWARE_VOLUMEVISUALIZER_H
#define SYNTHIA_FIRMWARE_VOLUMEVISUALIZER_H
#include "Visualizer.h"

class VolumeVisualizer : public Visualizer<uint8_t> {
protected:
	void visualize() override;
};


#endif //SYNTHIA_FIRMWARE_VOLUMEVISUALIZER_H
