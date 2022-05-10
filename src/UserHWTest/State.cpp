#include "State.h"
#include "Test.h"
#include <utility>
#include <Synthia.h>

UserHWTest::State::State(UserHWTest::Test* test) : test(test){

}

void UserHWTest::State::done(){
	test->testDone();
}

void UserHWTest::State::play(File file){
	test->playback.play(std::move(file));
}

bool UserHWTest::State::isPlaying(){
	return test->playback.isPlaying();
}

void UserHWTest::State::showText(std::string text){
	test->showText(std::move(text));
}

void UserHWTest::State::clearText(){
	test->clearText();
}

void UserHWTest::State::talk(){
	test->talk();
}
