#ifndef SYNTHIA_FIRMWARE_OUTRO_H
#define SYNTHIA_FIRMWARE_OUTRO_H

#include <Loop/LoopListener.h>
#include <Input/InputListener.h>
#include "../State.h"
#include "../../Timeout.h"

namespace UserHWTest {
	class Outro : public State, public LoopListener, private InputListener {
	public:
		Outro(Test* test);

		void start() override;
		void stop() override;

		void loop(uint micros) override;

	private:
		void buttonPressed(uint i) override;

		Timeout* timeout = nullptr;

	};
}


#endif //SYNTHIA_FIRMWARE_OUTRO_H
