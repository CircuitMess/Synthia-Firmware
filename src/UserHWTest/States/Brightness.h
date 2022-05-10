#ifndef SYNTHIA_FIRMWARE_BRIGHTNESS_H
#define SYNTHIA_FIRMWARE_BRIGHTNESS_H

#include <Input/SliderInput.h>
#include <Input/InputListener.h>
#include <Devices/Matrix/MatrixAnimGIF.h>
#include "../State.h"

namespace UserHWTest {
	class Brightness : public State, private SliderListener, private InputListener, public LoopListener {
	public:
		Brightness(Test* test);

		void loop(uint micros) override;

		void start() override;
		void stop() override;

	private:
		enum { Talking, Waiting } state = Talking;
		MatrixAnimGIF loadingAnim;

		void buttonPressed(uint i) override;
		void rightPotMove(uint8_t value) override;

		uint8_t brightness;

		void setBrightness(uint8_t value);
	};
}


#endif //SYNTHIA_FIRMWARE_BRIGHTNESS_H
