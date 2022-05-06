#ifndef SYNTHIA_FIRMWARE_SAVEVISUALIZER_H
#define SYNTHIA_FIRMWARE_SAVEVISUALIZER_H

#include "Visualizer.h"
#include <Devices/Matrix/MatrixAnimGIF.h>

enum SaveStep{
	SlotSelect, ActionSelect, Confirmation, Wait
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
	void onStart() override;
	void onStop() override;
private:
	MatrixAnimGIF slotAnim;
	MatrixAnimGIF yes;
	MatrixAnimGIF no;
	MatrixAnimGIF load;
	MatrixAnimGIF save;
	MatrixAnimGIF diskette;
};


#endif //SYNTHIA_FIRMWARE_SAVEVISUALIZER_H
