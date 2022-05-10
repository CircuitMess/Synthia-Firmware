#ifndef SYNTHIA_FIRMWARE_OUTRO_H
#define SYNTHIA_FIRMWARE_OUTRO_H

#include <Loop/LoopListener.h>
#include "../State.h"

namespace UserHWTest {
	class Outro : public State, public LoopListener {
	public:
		Outro(Test* test);

		void start() override;
		void stop() override;

		void loop(uint micros) override;
	};
}


#endif //SYNTHIA_FIRMWARE_OUTRO_H
