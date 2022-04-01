#ifndef SYNTHIA_FIRMWARE_SAVEVISUALIZER_H
#define SYNTHIA_FIRMWARE_SAVEVISUALIZER_H
#include "Visualizer.h"


struct SaveVisData {
	enum {
		SlotSelect, ActionSelect, Confirmation
	} state;
	uint8_t selection;
};

class SaveVisualizer : public Visualizer<SaveVisData> {
protected:
	void visualize() override;
};


#endif //SYNTHIA_FIRMWARE_SAVEVISUALIZER_H