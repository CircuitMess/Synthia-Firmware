#include "EditSlot.h"
#include <Audio/Effects/LowPass.h>
#include <Audio/Effects/HighPass.h>
#include <Audio/Effects/Reverb.h>
#include <Audio/Effects/BitCrusher.h>

#include <utility>

EditSlot::EditSlot(const SlotConfig& config, File file) : config(config), speeder(nullptr), effector(nullptr){

	playback = new PlaybackSlot(std::move(file));

	speeder.setSource(&playback->getSource());
	effector.setSource(&speeder);

	effects[0] = new LowPass();
	effects[1] = new HighPass();
	effects[2] = new Reverb();
	effects[3] = new BitCrusher();

	for(auto& effect : effects){
		effector.addEffect(effect);
	}

	for(int i = 0; i < (uint8_t) EffectData::Type::COUNT; i++){
		setEffect((EffectData::Type) i, config.effects[i].intensity);
	}

	setSpeed(config.speed);
}

EditSlot::~EditSlot(){
	for(auto effect: effects){
		delete effect;
	}
	delete playback;
}

void EditSlot::setEffect(EffectData::Type type, uint8_t intensity){
	switch(type){
		case EffectData::Type::COUNT:
			return;
		case EffectData::Type::VOLUME:
			playback->getSource().setVolume(intensity);
			break;
		default:
			effects[uint8_t(type)]->setIntensity(intensity);
			break;
	}

	config.effects[(uint8_t) type].intensity = intensity;
}

void EditSlot::setSpeed(uint8_t speed){
	speeder.setModifier(speed);
	config.speed = speed;
}

void EditSlot::setSample(Sample::Type type, File file){
	//don't do anything if sample wasn't changed (except for recordings, which can be modified)
	if(config.sample.type == type && type != Sample::Type::RECORDING) return;

	config.sample.sample = sample;
	delete playback;

	playback = new PlaybackSlot(std::move(file));
	speeder.setSource(&playback->getSource());
}

Generator& EditSlot::getGenerator(){
	return effector;
}

void EditSlot::seek(size_t pos, SeekMode mode){
	playback->seek(pos, mode);
}

SlotConfig EditSlot::getConfig(){
	return config;
}
