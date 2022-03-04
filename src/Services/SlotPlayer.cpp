#include "SlotPlayer.h"
#include <JayD.h>
#include "../AudioSystem/PlaybackSystem.h"

SlotPlayer slotPlayer;

void SlotPlayer::start(){
	//TODO - zamjeni jayd input sa input klasom
	InputJayD::getInstance()->addListener(this);
	enable();
}

void SlotPlayer::buttonPress(uint8_t id){
	if(!enabled) return;
	//TODO - zamjeni jayd pin defineove sa synthijinim
	switch(id){
		case BTN_L1:
			playOnInput(0);
			break;
		case BTN_L2:
			playOnInput(1);
			break;
		case BTN_L3:
			playOnInput(2);
			break;
		case BTN_R1:
			playOnInput(3);
			break;
		case BTN_R2:
			playOnInput(4);
			break;
		default:
			break;
	}

}

void SlotPlayer::enable(){
	enabled = true;
}

void SlotPlayer::disable(){
	enabled = false;
}

void SlotPlayer::play(uint8_t slot){
	playbackSystem.play(slot);
	//TODO - add RGBSlot blue blink
}

void SlotPlayer::playOnInput(uint8_t slot){
	playbackSystem.play(slot);
	//TODO - add RGBSlot green blink
}
