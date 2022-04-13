#ifndef SYNTHIA_FIRMWARE_SAMPLEVISUALIZER_H
#define SYNTHIA_FIRMWARE_SAMPLEVISUALIZER_H

#include "Visualizer.h"
#include "../Model/Sample.hpp"
#include <Devices/Matrix/MatrixAnimGIF.h>

class SampleVisualizer : public Visualizer<Sample::SampleType>{
protected:
public:
	SampleVisualizer();

protected:
	void visualize() override;

	void onStart() override;
	void onStop() override;

private:
	static const char* Anims[];
	std::vector<std::unique_ptr<MatrixAnimGIF>> anims;

};


#endif //SYNTHIA_FIRMWARE_SAMPLEVISUALIZER_H
