#ifndef SYNTHIA_FIRMWARE_SETBRIGHTNESS_H
#define SYNTHIA_FIRMWARE_SETBRIGHTNESS_H


#include <Loop/LoopListener.h>
#include <Input/InputListener.h>
#include <Input/SliderInput.h>
#include <Devices/Matrix/MatrixAnimGIF.h>
#include "State.h"
#include "../Visualization/RGBDemoAnim.h"

class SetBrightness : public State, public LoopListener, public InputListener, public SliderListener {
public:
	SetBrightness(State* parent);

private:
	MatrixAnimGIF trackAnim;
	RGBDemoAnim rgbTrackAnim;
	RGBDemoAnim rgbSlotAnim;

	void onStart() override;
	void onStop() override;
	void loop(uint micros) override;

	void buttonPressed(uint i) override;
	void leftPotMove(uint8_t value) override;
	void rightPotMove(uint8_t value) override;

	void setBrightness(uint8_t value);

	uint8_t brightness = 100;

};


#endif //SYNTHIA_FIRMWARE_SETBRIGHTNESS_H
