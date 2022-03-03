#include "EditSlot.h"
#include <Audio/Effects/LowPass.h>
#include <Audio/Effects/HighPass.h>
#include <Audio/Effects/Reverb.h>
#include <Audio/Effects/BitCrusher.h>

EditSlot::EditSlot(SlotConfig config) : config(config), sampleFile(openSample(config)), playback(new PlaybackSlot(RamFile::open(sampleFile))),
										speeder(&playback->getSource()), effector(&speeder){

	sampleFile.close();
	setSpeed(config.speed);
	effects[0] = new LowPass();
	effects[1] = new HighPass();
	effects[2] = new Reverb();
	effects[3] = new BitCrusher();

	for(auto &effect: effects){
		effector.addEffect(effect);
	}

	for(int i = 0; i < (uint8_t)EffectData::Type::COUNT - 1; i++){
		effects[i]->setIntensity(config.effects[i].intensity);
		setEffect((EffectData::Type)i, config.effects[i].intensity);
	}
	setEffect(EffectData::Type::VOLUME, config.effects[4].intensity);
}

EditSlot::~EditSlot(){
	for(auto effect: effects){
		delete effect;
	}
	delete playback;
}

void EditSlot::setEffect(EffectData::Type type, uint8_t intensity){
	switch(type){
		case EffectData::Type::VOLUME:
			playback->getSource().setVolume(intensity);
			break;
		case EffectData::Type::COUNT:
			break;
		default:
			effects[uint8_t(type)]->setIntensity(intensity);
			break;
	}
}

void EditSlot::setSpeed(uint8_t speed){
	speeder.setModifier(speed);
	config.speed = speed;
}

Generator &EditSlot::getGenerator(){
	return effector;
}

void EditSlot::seek(size_t pos, SeekMode mode){
	playback->seek(pos, mode);
}

SlotConfig EditSlot::getConfig(){
	return config;
}
