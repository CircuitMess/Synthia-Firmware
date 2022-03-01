#include <Arduino.h>
#include <CMAudio.h>
#include <CircuitOS.h>

//#include <Synthia.h>
//#include <Pins.hpp>

#include <JayD.h>
#include <Input/InputJayD.h>

#include "src/AudioSystem/PlaybackSystem.h"
#include <Loop/LoopManager.h>
PlaybackSystem* ps;

void setup(){
	Serial.begin(115200);
//	Synthia.begin();
	JayD.begin();

	ps = new PlaybackSystem();
	ps->init();

/*	Input::getInstance()->setBtnPressCallback(BTN_1, [](){ps->play(0);});
	Input::getInstance()->setBtnPressCallback(BTN_2, [](){ps->play(1);});
	Input::getInstance()->setBtnPressCallback(BTN_3, [](){ps->play(2);});
	Input::getInstance()->setBtnPressCallback(BTN_4, [](){ps->play(3);});
	Input::getInstance()->setBtnPressCallback(BTN_5, [](){ps->play(4);});*/

	InputJayD::getInstance()->setBtnPressCallback(BTN_L1, [](){ps->play(0);});
	InputJayD::getInstance()->setBtnPressCallback(BTN_L2, [](){ps->play(1);});
	InputJayD::getInstance()->setBtnPressCallback(BTN_L3, [](){ps->play(2);});
	InputJayD::getInstance()->setBtnPressCallback(BTN_R1, [](){ps->play(3);});
	InputJayD::getInstance()->setBtnPressCallback(BTN_R2, [](){ps->play(4);});
}


void loop(){
	LoopManager::loop();
}
