#ifndef SYNTHIA_FIRMWARE_TEST_H
#define SYNTHIA_FIRMWARE_TEST_H

#include <Arduino.h>
#include "State.h"
#include "../AudioSystem/SimplePlaybackSystem.h"
#include <Loop/LoopListener.h>
#include <Devices/Matrix/MatrixAnimGIF.h>

namespace UserHWTest {
	class Test : public LoopListener {
	friend State;

	public:
		Test(void (*doneCallback)());
		virtual ~Test();

		void start();

		void loop(uint micros) override;

	private:
		SimplePlaybackSystem playback;

		static State* (*LaunchState[])(UserHWTest::Test* test);
		uint8_t stateIndex = 0;
		State* state = nullptr;

		void testDone();
		void (*doneCallback)();

		void showText(std::string text);
		void clearText();
		std::string text;
		uint32_t textScrollTime;
		int16_t textScrollCursor;

		MatrixAnimGIF talkAnim;
		void talk();

	};
}


#endif //SYNTHIA_FIRMWARE_TEST_H
