#include "SetBrightness.h"
#include "../Visualization/LEDStrip.h"
#include "TrackEditState.h"
#include <Synthia.h>
#include <FS/RamFile.h>
#include <Util/HWRevision.h>

SetBrightness::SetBrightness(State* parent) : State(parent), trackAnim(RamFile::open(SPIFFS.open("/GIF/Swipe.gif")), &Synthia.TrackMatrix), rgbTrackAnim(Synthia.TrackRGB), rgbSlotAnim(Synthia.SlotRGB){

}

void SetBrightness::onStart(){
	Synthia.getInput()->addListener(this);
	Sliders.addListener(this);

	Synthia.clearMatrices();

	setBrightness(Settings.get().brightness);

	Synthia.TrackMatrix.startAnimation(&trackAnim);
	Synthia.TrackRGB.startAnimation(&rgbTrackAnim);
	Synthia.SlotRGB.startAnimation(&rgbSlotAnim);
}

void SetBrightness::onStop(){
	Synthia.getInput()->removeListener(this);
	Sliders.removeListener(this);

	trackAnim.stop();
	rgbTrackAnim.stop();
	rgbSlotAnim.stop();
	Synthia.clearMatrices();

	Settings.get().brightness = brightness;
	Settings.store();
}

void SetBrightness::loop(uint micros){

}

void SetBrightness::buttonPressed(uint i){
	pop();
}

void SetBrightness::leftPotMove(uint8_t value){
	setBrightness(value);
}

void SetBrightness::rightPotMove(uint8_t value){
	setBrightness(value);
}

void SetBrightness::setBrightness(uint8_t value){
	brightness = value;
	Settings.get().brightness = value;

	const auto mapped = map(value, 0, 255, 50, 150);
	Synthia.getCharlie().setBrightness(mapped);

	if(HWRevision::get() > 0){
		Synthia.SlotRGB.setBrightness(brightness);
		Synthia.TrackRGB.setBrightness(brightness);
	}

	LEDStrip.setRight(value);
	LEDStrip.setLeft(value);
}
