#include <Loop/LoopManager.h>
#include <Synthia.h>
#include "Test.h"
#include "States/Intro.h"
#include "States/Buttons.h"
#include "States/Sliders.h"
#include "States/Encoders.h"
#include "States/Outro.h"
#include "States/Brightness.h"

UserHWTest::State* (*UserHWTest::Test::LaunchState[])(UserHWTest::Test* test) = {
		[](UserHWTest::Test* test) -> UserHWTest::State* { return new UserHWTest::Intro(test); },
		[](UserHWTest::Test* test) -> UserHWTest::State* { return new UserHWTest::Buttons(test); },
		[](UserHWTest::Test* test) -> UserHWTest::State* { return new UserHWTest::Sliders(test); },
		[](UserHWTest::Test* test) -> UserHWTest::State* { return new UserHWTest::Encoders(test); },
		[](UserHWTest::Test* test) -> UserHWTest::State* { return new UserHWTest::Brightness(test); },
		[](UserHWTest::Test* test) -> UserHWTest::State* { return new UserHWTest::Outro(test); }
};


UserHWTest::Test::Test() : talkAnim(RamFile::open(SPIFFS.open("/talk.gif")), &Synthia.TrackMatrix){
	playback.setVolume(50);
}

UserHWTest::Test::~Test(){
	LoopManager::removeListener(this);
}

void UserHWTest::Test::start(){
	if(state) return;

	playback.begin();

	state = LaunchState[0](this);
	state->start();
}

void UserHWTest::Test::testDone(){
	if(state){
		state->stop();
	}
	delete state;
	state = nullptr;

	clearText();
	Synthia.TrackMatrix.stopAnimations();
	Synthia.TrackMatrix.clear();

	stateIndex++;
	if(stateIndex == sizeof(LaunchState) / sizeof(LaunchState[0])){
		exit();
		return;
	}

	state = LaunchState[stateIndex](this);
	state->start();
}

void UserHWTest::Test::showText(std::string text){
	this->text = text;

	Synthia.TrackMatrix.stopAnimations();
	Synthia.TrackMatrix.setFont(Matrix::SMALL);
	Synthia.TrackMatrix.clear();
	Synthia.TrackMatrix.drawString(0, 0, text.c_str());
	Synthia.TrackMatrix.push();

	textScrollCursor = -Synthia.TrackMatrix.getWidth();

	textScrollTime = millis();
	LoopManager::addListener(this);
}

void UserHWTest::Test::clearText(){
	text.clear();
	LoopManager::removeListener(this);
}

void UserHWTest::Test::loop(uint micros){
	if(text.empty()){
		LoopManager::removeListener(this);
		return;
	}

	if(millis() - textScrollTime <= 100) return;

	textScrollTime = millis();
	textScrollCursor++;
	if(textScrollCursor >= (int16_t) (text.size() * (3 + 1) - 1)){
		textScrollCursor = -Synthia.TrackMatrix.getWidth();
	}

	Synthia.TrackMatrix.clear();
	Synthia.TrackMatrix.setFont(Matrix::SMALL);
	Synthia.TrackMatrix.drawString(-textScrollCursor, 5, text.c_str());
	Synthia.TrackMatrix.push();
}

void UserHWTest::Test::talk(){
	clearText();
	Synthia.TrackMatrix.startAnimation(&talkAnim);
}

void UserHWTest::Test::exit(){
	Settings.get().tested = true;
	Settings.store();

	Synthia.clearMatrices();
	playback.stop();

	ESP.restart();
}
