#ifndef SYNTHIA_FIRMWARE_EFFECTVISUALIZER_H
#define SYNTHIA_FIRMWARE_EFFECTVISUALIZER_H

#include "Visualizer.h"
#include "../Model/EffectData.hpp"

class EffectVisualizer : public Visualizer<EffectData> {
protected:
	void visualize() override;
	void onStart() override;
	void onStop() override;
};


#endif //SYNTHIA_FIRMWARE_EFFECTVISUALIZER_H
