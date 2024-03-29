#include "EditSlot.h"
#include <Audio/Effects/LowPass.h>
#include <Audio/Effects/HighPass.h>
#include <Audio/Effects/Reverb.h>
#include <Audio/Effects/BitCrusher.h>

#include <utility>

EditSlot::EditSlot(const SlotConfig& config, File file) : config(config), speeder(nullptr), effector(nullptr), playback(new PlaybackSlot(std::move(file))){
	speeder.setSource(&playback->getGenerator());
	effector.setSource(&speeder);

	effects[0] = new BitCrusher();
	effects[1] = new Reverb();
	effects[2] = new LowPass();
	effects[3] = new HighPass();


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
}

void EditSlot::setEffect(EffectData::Type type, uint8_t intensity){
	switch(type){
		case EffectData::Type::COUNT:
			return;
		case EffectData::Type::VOLUME:
			playback->getSource().setVolume(intensity);
			break;
		default:
			effects[(uint8_t) type - 1]->setIntensity(intensity);
			break;
	}

	config.effects[(uint8_t) type].intensity = intensity;
}

void EditSlot::setSpeed(uint8_t speed){
	speeder.setModifier(speed);
	config.speed = speed;
}

Generator& EditSlot::getGenerator(){
	return effector;
}

void EditSlot::seek(size_t pos, SeekMode mode){
	playback->seek(pos, mode);
	speeder.resetBuffers();
}

SlotConfig EditSlot::getConfig(){
	return config;
}
