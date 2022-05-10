#ifndef SYNTHIA_FIRMWARE_SLIDERS_H
#define SYNTHIA_FIRMWARE_SLIDERS_H

#include <Input/SliderInput.h>
#include <Input/InputListener.h>
#include "../State.h"

namespace UserHWTest {
	class Sliders : public State, public LoopListener, private InputListener, private SliderListener {
	public:
		Sliders(Test* test);

		void start() override;
		void stop() override;

		void loop(uint micros) override;

	private:
		enum { Talking, Waiting } state = Talking;

		struct {
			int32_t max = -1, min = -1;
		} sliderL, sliderR, showL, showR;

		uint32_t sliderSamplesL = 0, sliderSamplesR = 0;
		uint8_t sampleCount = 0;
		const uint8_t totalSampleCount = 10;

		void leftPotMove(uint8_t value) override;
		void rightPotMove(uint8_t value) override;

		void buttonPressed(uint i) override;

		void lightSlider(uint8_t index);

	};
}


#endif //SYNTHIA_FIRMWARE_SLIDERS_H
