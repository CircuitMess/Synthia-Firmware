#ifndef SYNTHIA_FIRMWARE_PLAYBACKSTATE_H
#define SYNTHIA_FIRMWARE_PLAYBACKSTATE_H

#include <Arduino.h>
#include "State.h"
#include "../Model/Track.h"
#include "../Visualization/TrackVisualizer.h"

class PlaybackState : public State, public LoopListener, public InputListener, public SliderListener, public EncoderListener {
public:
	PlaybackState(const TrackTimeline& timeline, uint8_t& tempo);

	void loop(uint micros) override;

protected:
	void onStart() override;
	void onStop() override;

private:
	TrackTimeline timeline;
	uint8_t& tempo;
	uint8_t cursor;

	TrackVisualizer vis;
	void pushTrackVis();

	uint32_t lastStepTime;
	uint32_t stepDuration;
	void playStep(uint8_t step);

	bool disabledSlots[5] = { false };
	void buttonHeld(uint i) override;

	void leftPotMove(uint8_t value) override;
	void rightPotMove(uint8_t value) override;

	void rightEncMove(int8_t amount) override;
};


#endif //SYNTHIA_FIRMWARE_PLAYBACKSTATE_H
