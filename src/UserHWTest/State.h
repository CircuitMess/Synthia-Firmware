#ifndef SYNTHIA_FIRMWARE_STATE_H
#define SYNTHIA_FIRMWARE_STATE_H

#include <Arduino.h>
#include <FS.h>

namespace UserHWTest {
	class Test;

	class State {
	public:
		State(UserHWTest::Test* test);

		virtual void start() = 0;
		virtual void stop() = 0;

	protected:
		void done();

		void play(File file);
		bool isPlaying();

		void showText(std::string text);
		void clearText();

		void talk();

	private:
		Test* test;

	};
}


#endif //SYNTHIA_FIRMWARE_STATE_H
