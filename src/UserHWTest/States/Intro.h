#ifndef SYNTHIA_FIRMWARE_INTRO_H
#define SYNTHIA_FIRMWARE_INTRO_H

#include <Loop/LoopListener.h>
#include "../State.h"

namespace UserHWTest{
	class Intro : public State, public LoopListener {
	public:
		explicit Intro(Test* test);

		void start() override;
		void stop() override;

		void loop(uint micros) override;
	};
}


#endif //SYNTHIA_FIRMWARE_INTRO_H
