#ifndef SYNTHIA_FIRMWARE_SAVEVISUALIZER_H
#define SYNTHIA_FIRMWARE_SAVEVISUALIZER_H

#include "Visualizer.h"
#include <Devices/Matrix/MatrixAnimGIF.h>

enum SaveStep{
	SlotSelect, ActionSelect, Confirmation
};

struct SaveVisData {
	SaveStep state;
	uint8_t selection;
};

class SaveVisualizer : public Visualizer<SaveVisData> {
public:
	SaveVisualizer();

protected:
	void visualize() override;

private:
	MatrixAnimGIF slotAnim;
};


#endif //SYNTHIA_FIRMWARE_SAVEVISUALIZER_H
