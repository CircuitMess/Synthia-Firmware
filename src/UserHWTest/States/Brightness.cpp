#include <SPIFFS.h>
#include <Synthia.h>
#include <FS/RamFile.h>
#include <Loop/LoopManager.h>
#include <Util/HWRevision.h>
#include "Brightness.h"
#include "../../Visualization/LEDStrip.h"

UserHWTest::Brightness::Brightness(UserHWTest::Test* test) : State(test), loadingAnim(RamFile::open(SPIFFS.open("/GIF/Loading.gif")), &Synthia.TrackMatrix){

}

void UserHWTest::Brightness::start(){
	play(SPIFFS.open("/HWTestSamples/brightness.aac"));
	talk();

	LoopManager::addListener(this);
	::Sliders.addListener(this);
	Synthia.getInput()->addListener(this);

	for(int i = 0; i < 8; i++){
		Synthia.SlidersMatrix.drawPixel(0, i, MatrixPixel::White);
	}
	Synthia.SlidersMatrix.push();

	setBrightness(Settings.get().brightness);
}

void UserHWTest::Brightness::stop(){
	loadingAnim.stop();

	LoopManager::removeListener(this);
	::Sliders.removeListener(this);
	Synthia.getInput()->removeListener(this);

	Synthia.clearMatrices();

	Settings.get().brightness = brightness;
	Settings.store();
}

void UserHWTest::Brightness::loop(uint micros){
	if(isPlaying() || state != Talking) return;

	state = Waiting;
	Synthia.TrackMatrix.stopAnimations();
	Synthia.TrackMatrix.startAnimation(&loadingAnim);
}

void UserHWTest::Brightness::buttonPressed(uint i){
	done();
}

void UserHWTest::Brightness::rightPotMove(uint8_t value){
	setBrightness(value);
}

void UserHWTest::Brightness::setBrightness(uint8_t value){
	brightness = value;

	const auto mapped = map(value, 0, 255, 50, 150);
	Synthia.getCharlie().setBrightness(mapped);

	if(HWRevision::get() > 0){
		Synthia.SlotRGB.setBrightness(mapped);
		Synthia.TrackRGB.setBrightness(mapped);
	}

	LEDStrip.setRight(value);
}
