#include <SPIFFS.h>
#include <Loop/LoopManager.h>
#include <Settings.h>
#include "Intro.h"

UserHWTest::Intro::Intro(UserHWTest::Test* test) : State(test){

}

void UserHWTest::Intro::start(){
	play(SPIFFS.open(Settings.get().tested ? "/HWTestSamples/intro2.aac" : "/HWTestSamples/intro.aac"));
	talk();

	LoopManager::addListener(this);
}

void UserHWTest::Intro::stop(){
	LoopManager::removeListener(this);
}

void UserHWTest::Intro::loop(uint micros){
	if(isPlaying()) return;
	done();
}
