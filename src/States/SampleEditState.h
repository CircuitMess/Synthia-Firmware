#ifndef SYNTHIA_FIRMWARE_SAMPLEEDITSTATE_H
#define SYNTHIA_FIRMWARE_SAMPLEEDITSTATE_H

#include "State.h"
#include "../Visualization/SampleVisualizer.h"
#include "../Visualization/EffectVisualizer.h"
#include "../Visualization/SampleSpeedVisualizer.h"
#include <Synthia.h>
#include <JPEGDecoder.h>
#include "../AudioSystem/EditSlot.h"
#include "../AudioSystem/Recorder.h"

class SampleEditState : public State, public InputListener, public EncoderListener, public SliderListener, public LoopListener {
public:
	SampleEditState(State* parent, uint8_t slot);
	virtual ~SampleEditState();

	void loop(uint micros) override;

protected:
	void onStart() override;
	void onStop() override;

private:
	void buttonHeld(uint i) override;
	void leftEncMove(int8_t amount) override;
	void rightEncMove(int8_t amount) override;
	void leftPotMove(uint8_t value) override;
	void rightPotMove(uint8_t value) override;
	void buttonReleased(uint i) override;
	void buttonPressed(uint i) override;

	void saveRecording(SlotConfig* other);

	uint8_t slot = 0;
	EditSlot *editSlot = nullptr;
	SlotConfig config;
	SampleVisualizer sampleVis;
	EffectVisualizer effectVis;
	SampleSpeedVisualizer speedVis;
	Recorder* recorder = nullptr;

	Task* load;
	bool recorded = false;

	EffectData::Type selectedEffect = EffectData::Type::VOLUME;

	File rawSamples[5];

};


#endif //SYNTHIA_FIRMWARE_SAMPLEEDITSTATE_H
