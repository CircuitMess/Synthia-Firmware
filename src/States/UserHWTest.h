#ifndef SYNTHIA_FIRMWARE_USERHWTEST_H
#define SYNTHIA_FIRMWARE_USERHWTEST_H

#include "State.h"
#include <Synthia.h>
#include <JPEGDecoder.h>
#include <Devices/Matrix/MatrixAnimGIF.h>
#include <Settings.h>
#include <Audio/OutputI2S.h>
#include <Audio/SourceAAC.h>
#include <Data/FileDataSource.h>
#include "../AudioSystem/PlaybackSystemAAC.h"

class UserHWTest : public State, public SliderListener, public EncoderListener, public InputListener, public LoopListener {
public:
	UserHWTest(void (*callback)() = nullptr);
	void loop(uint micros) override;

protected:
	void onStart() override;
	void onStop() override;

private:
	void buttonPressed(uint i) override;
	void leftEncMove(int8_t amount) override;
	void rightEncMove(int8_t amount) override;
	void leftPotMove(uint8_t value) override;
	void rightPotMove(uint8_t value) override;

	enum class TestState {
		Intro, Buttons, Sliders, Encoders, Outro
	} state = TestState::Intro;

	void startState(TestState state);
	void stopState(TestState state);
	MatrixAnimGIF talking;

	void (*doneCallback)() = nullptr;

	bool buttonsPressed[7] = {false};
	uint8_t pressedCount = 0;
	uint32_t textScrollTimer = 0;
	int textCursor = 0;
	const char* scrollingText;

	SlidersCalibration calibration;

	uint8_t knobX = 0;
	uint8_t knobY = 0;

	File audioFile;
	PlaybackSystemAAC playback;
	void playFile(const char* path);

	uint8_t btnToIndex(uint32_t i);
};


#endif //SYNTHIA_FIRMWARE_USERHWTEST_H
