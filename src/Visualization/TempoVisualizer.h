#ifndef SYNTHIA_FIRMWARE_TEMPOVISUALIZER_H
#define SYNTHIA_FIRMWARE_TEMPOVISUALIZER_H

#include "Visualizer.h"
#include "TempoAnim.h"
#include "MetronomeAnim.h"

class TempoVisualizer : public Visualizer<uint8_t> {
public:
	TempoVisualizer();

protected:
	void visualize() override;
	void onStop() override;

private:
	TempoAnim anim;
	MetronomeAnim metronome;
};


#endif //SYNTHIA_FIRMWARE_TEMPOVISUALIZER_H
