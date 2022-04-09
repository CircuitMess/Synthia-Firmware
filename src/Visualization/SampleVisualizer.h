#ifndef SYNTHIA_FIRMWARE_SAMPLEVISUALIZER_H
#define SYNTHIA_FIRMWARE_SAMPLEVISUALIZER_H

#include "Visualizer.h"
#include "../Model/Sample.hpp"

class SampleVisualizer : public Visualizer<Sample::SampleType>{
protected:
	void visualize() override;
	void onStop() override;
	void onStart() override;
};


#endif //SYNTHIA_FIRMWARE_SAMPLEVISUALIZER_H
