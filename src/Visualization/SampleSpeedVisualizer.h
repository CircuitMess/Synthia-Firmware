#ifndef SYNTHIA_FIRMWARE_SAMPLESPEEDVISUALIZER_H
#define SYNTHIA_FIRMWARE_SAMPLESPEEDVISUALIZER_H

#include <Devices/Matrix/MatrixAnimGIF.h>
#include "Visualizer.h"

class SampleSpeedVisualizer : public Visualizer<uint8_t> {
public:
	SampleSpeedVisualizer();

protected:
	void visualize() override;
	void onStop() override;
	void onStart() override;

private:
	std::unique_ptr<MatrixAnimGIF> anim;
};


#endif //SYNTHIA_FIRMWARE_SAMPLESPEEDVISUALIZER_H
