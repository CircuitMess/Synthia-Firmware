#include <SPIFFS.h>
#include <Synthia.h>
#include "Encoders.h"

UserHWTest::Encoders::Encoders(UserHWTest::Test* test) : State(test){

}

void UserHWTest::Encoders::start(){
	play(SPIFFS.open("/HWTestSamples/knobs.aac"));

	knobX = Synthia.TrackMatrix.getWidth() / 2;
	knobY = Synthia.TrackMatrix.getHeight() / 2;
	Synthia.TrackMatrix.drawPixel(knobX, knobY, MatrixPixel::White);
	Synthia.TrackMatrix.push();

	::Encoders.addListener(this);
	Synthia.getInput()->addListener(this);
}

void UserHWTest::Encoders::stop(){
	::Encoders.removeListener(this);
	Synthia.getInput()->removeListener(this);
}

void UserHWTest::Encoders::leftEncMove(int8_t amount){
	Synthia.TrackMatrix.drawPixel(knobX, knobY, MatrixPixel::Off);
	if(knobX == 0 && amount < 0){
		knobX = Synthia.TrackMatrix.getWidth() - 1;
	}else{
		knobX = (knobX + amount) % Synthia.TrackMatrix.getWidth();
	}

	Synthia.TrackMatrix.drawPixel(knobX, knobY, MatrixPixel::White);
	Synthia.TrackMatrix.push();
}

void UserHWTest::Encoders::rightEncMove(int8_t amount){
	Synthia.TrackMatrix.drawPixel(knobX, knobY, MatrixPixel::Off);
	if(knobY == 0 && amount < 0){
		knobY = Synthia.TrackMatrix.getHeight() - 1;
	}else{
		knobY = (knobY + amount) % Synthia.TrackMatrix.getHeight();
	}

	Synthia.TrackMatrix.drawPixel(knobX, knobY, MatrixPixel::White);
	Synthia.TrackMatrix.push();
}

void UserHWTest::Encoders::buttonPressed(uint i){
	done();
}
