#include "EditSlot.h"
#include <Audio/Effects/LowPass.h>
#include <Audio/Effects/HighPass.h>
#include <Audio/Effects/Reverb.h>
#include <Audio/Effects/BitCrusher.h>

EditSlot::EditSlot(SlotConfig config) : sample(openSample(config)), playback(RamFile::open(sample)), speeder(&playback.getSource()),
										effector(&speeder){
	speeder.setSpeed(config.speed);
	effects[0] = new LowPass();
	effects[1] = new HighPass();
	effects[2] = new Reverb();
	effects[3] = new BitCrusher();
	for(int i = 0; i < (uint8_t)EffectData::Type::COUNT - 1; i++){
		effects[i]->setIntensity(config.effects[i].intensity);
	}
}

EditSlot::~EditSlot(){
	for(auto effect: effects){
		delete effect;
	}
}

void EditSlot::setEffect(EffectData::Type type, uint8_t intensity){
	switch(type){
		case EffectData::Type::VOLUME:
			playback.getSource().setVolume(intensity);
			break;
		case EffectData::Type::COUNT:
			break;
		default:
			effects[uint8_t(type)]->setIntensity(intensity);
			break;
	}
}

void EditSlot::setSpeed(uint8_t speed){
	speeder.setSpeed((float)speed / 255.0);
}

Generator & EditSlot::getGenerator(){
	return playback.getGenerator();
}

void EditSlot::seek(size_t pos, SeekMode mode){
	playback.seek(pos, mode);
}
