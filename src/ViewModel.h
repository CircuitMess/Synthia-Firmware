#ifndef SYNTHIA_FIRMWARE_VIEWMODEL_H
#define SYNTHIA_FIRMWARE_VIEWMODEL_H

#include "Visualizer.hpp"

struct TrackVis{
	uint8_t TrackSlotData;
	uint8_t cursorPosition;
};
struct SaveVis{
	enum State{ SlotSelect, ActionSelect, Confirmation};
	uint8_t selection;
};
struct EffectVisualizer{
	enum Effect{ volume, highpass, lowpass, echo};
	uint8_t intensity;
};

class ViewModel : public Visualizer<ViewModel>{
public:
	void propChanged(ViewModel) override;
private:
};

#endif //SYNTHIA_FIRMWARE_VIEWMODEL_H
