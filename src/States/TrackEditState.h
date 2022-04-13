#ifndef SYNTHIA_FIRMWARE_TRACKEDITSTATE_H
#define SYNTHIA_FIRMWARE_TRACKEDITSTATE_H

#include "State.h"
#include <Synthia.h>
#include <cstdlib>
#include "../Model/Track.h"
#include "../Visualization/TempoVisualizer.h"
#include "../Visualization/VolumeVisualizer.h"
#include "../Visualization/TrackVisualizer.h"
#include "../ButtonActionSystem.h"
#include <Input/InputListener.h>

class TrackEditState : public State, public EncoderListener, public SliderListener, public InputListener {
public:
	TrackEditState();

	void setTrack(Track track);
	const Track& getTrack();

protected:
	void onStart() override;
	void onStop() override;

private:
	void buttonPressed(uint i) override;
	void buttonHeld(uint i) override;
	void click(uint8_t i);
	void doubleClick(uint8_t i);

	void leftEncMove(int8_t amount) override;
	void rightEncMove(int8_t amount) override;

	void leftPotMove(uint8_t value) override;
	void rightPotMove(uint8_t value) override;

	Track track = { };
	static uint8_t cursor;

	TempoVisualizer tempoVis;
	VolumeVisualizer volumeVis;
	TrackVisualizer trackVis;

	uint32_t clickTimes[5] = {0};
	uint32_t doubleClickInterval = 500;//[ms]

	bool slotEraser[5] = { false };

	void pushTrackVis();
	void buttonReleased(uint i) override;
};


#endif //SYNTHIA_FIRMWARE_TRACKEDITSTATE_H
