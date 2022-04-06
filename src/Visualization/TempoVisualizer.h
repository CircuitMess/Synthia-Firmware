#ifndef SYNTHIA_FIRMWARE_TEMPOVISUALIZER_H
#define SYNTHIA_FIRMWARE_TEMPOVISUALIZER_H

#include "Visualizer.h"

class TempoAnim;

class TempoVisualizer : public Visualizer<uint8_t> {
protected:
	void visualize() override;
private:
	static TempoAnim* anim;
};


#endif //SYNTHIA_FIRMWARE_TEMPOVISUALIZER_H
