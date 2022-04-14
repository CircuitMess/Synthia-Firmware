#ifndef SYNTHIA_FIRMWARE_VOLUMEVISUALIZER_H
#define SYNTHIA_FIRMWARE_VOLUMEVISUALIZER_H

#include "Visualizer.h"
#include "Devices/Matrix/MatrixAnimGIF.h"

class VolumeVisualizer : public Visualizer<uint8_t> {
public:
	VolumeVisualizer();

protected:
	void visualize() override;
	void onStart() override;
	void onStop() override;

private:
	MatrixAnimGIF anim;

};


#endif //SYNTHIA_FIRMWARE_VOLUMEVISUALIZER_H
