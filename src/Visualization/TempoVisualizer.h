#ifndef SYNTHIA_FIRMWARE_TEMPOVISUALIZER_H
#define SYNTHIA_FIRMWARE_TEMPOVISUALIZER_H

#include "Visualizer.h"
#include "TempoAnim.h"

class TempoVisualizer : public Visualizer<uint8_t> {
public:
	TempoVisualizer();

protected:
	void visualize() override;

private:
	TempoAnim anim;
};


#endif //SYNTHIA_FIRMWARE_TEMPOVISUALIZER_H
