#ifndef SYNTHIA_FIRMWARE_BUTTONS_H
#define SYNTHIA_FIRMWARE_BUTTONS_H

#include <Loop/LoopListener.h>
#include <Input/InputListener.h>
#include "../State.h"

namespace UserHWTest{
	class Buttons : public State, public LoopListener, private InputListener {
	public:
		Buttons(Test* test);

		void start() override;
		void stop() override;

		void loop(uint micros) override;
	private:
		void buttonPressed(uint i) override;

	private:
		enum { Talking, Waiting } state = Talking;
		bool clicked[7] = { false };
		uint8_t clickCount = 0;

	};
}


#endif //SYNTHIA_FIRMWARE_BUTTONS_H
