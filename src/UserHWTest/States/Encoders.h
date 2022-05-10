#ifndef SYNTHIA_FIRMWARE_ENCODERS_H
#define SYNTHIA_FIRMWARE_ENCODERS_H

#include <Input/EncoderInput.h>
#include "../State.h"

namespace UserHWTest{
	class Encoders : public State, private EncoderListener, private InputListener {
	public:
		Encoders(Test* test);

		void start() override;
		void stop() override;

	private:
		uint8_t knobX = 0;
		uint8_t knobY = 0;

		void leftEncMove(int8_t amount) override;
		void rightEncMove(int8_t amount) override;

		void buttonPressed(uint i) override;
	};
}


#endif //SYNTHIA_FIRMWARE_ENCODERS_H
