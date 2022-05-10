#include <SPIFFS.h>
#include <Loop/LoopManager.h>
#include "Outro.h"

UserHWTest::Outro::Outro(UserHWTest::Test* test) : State(test){

}

void UserHWTest::Outro::start(){
	play(SPIFFS.open("/HWTestSamples/outro.aac"));
	talk();

	LoopManager::addListener(this);
}

void UserHWTest::Outro::stop(){
	LoopManager::removeListener(this);
}

void UserHWTest::Outro::loop(uint micros){
	if(isPlaying()) return;
	done();
}
