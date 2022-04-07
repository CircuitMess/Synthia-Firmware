#ifndef SYNTHIA_FIRMWARE_SAVESTATE_H
#define SYNTHIA_FIRMWARE_SAVESTATE_H

#include "TrackEditState.h"
#include "../Visualization/SaveVisualizer.h"
#include <Synthia.h>
#include <cstdlib>

class SaveState : public State, public EncoderListener, public InputListener, public LoopListener, public SliderListener {
public:
	SaveState(TrackEditState* trackEdit);
	void loop(uint micros) override;

protected:
	void onStart() override;
	void onStop() override;

private:
	void save();
	void load();

	void buttonPressed(uint i) override;
	void rightEncMove(int8_t amount) override;
	void leftEncMove(int8_t amount) override;
	void leftPotMove(uint8_t value) override;
	void rightPotMove(uint8_t value) override;

	TrackEditState* trackEdit = nullptr;

	SaveVisualizer visualizer;
	SaveStep step = SlotSelect;
	uint8_t selection = 0;
	uint8_t selectedSlot = 0;
	enum {
		Save, Load
	} selectedAction;

	static uint8_t currentSaveSlot;

	uint32_t inactiveTimer = 0;
	const uint32_t  inactiveTimeout = 5000; //5 seconds
};


#endif //SYNTHIA_FIRMWARE_SAVESTATE_H
