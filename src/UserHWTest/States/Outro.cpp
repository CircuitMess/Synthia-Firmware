#include <SPIFFS.h>
#include <Loop/LoopManager.h>
#include <Synthia.h>
#include "Outro.h"

UserHWTest::Outro::Outro(UserHWTest::Test* test) : State(test){

}

void UserHWTest::Outro::start(){
	play(SPIFFS.open("/HWTestSamples/outro.aac"));
	talk();

	LoopManager::addListener(this);

	if(Settings.get().tested){
		Synthia.getInput()->addListener(this);
	}
}

void UserHWTest::Outro::stop(){
	LoopManager::removeListener(this);
	Synthia.getInput()->removeListener(this);
}

void UserHWTest::Outro::loop(uint micros){
	if(isPlaying()) return;
	done();
}

void UserHWTest::Outro::buttonPressed(uint i){
	done();
}
