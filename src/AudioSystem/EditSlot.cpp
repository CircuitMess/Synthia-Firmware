#include "EditSlot.h"
#include <SPIFFS.h>
#include <Audio/Effects/LowPass.h>
#include <Audio/Effects/HighPass.h>
#include <Audio/Effects/Reverb.h>
#include <Audio/Effects/BitCrusher.h>

const char* sampleNames[] = {"KICK", "SNARE", "CLAP", "CLOSEDHIHAT", "OPENHIHAT", "RIMSHOT", "BASS", "RECORDING"};
//effect types LOWPASS, HIGHPASS, REVERB, BITCRUSHER, VOLUME, COUNT
EditSlot::EditSlot(SlotConfig config) : playback(openFile(config)), speeder(&playback.getSource()), effector(&speeder){
	speeder.setSpeed(config.speed);
	effects[0] = new LowPass();
	effects[1] = new HighPass();
	effects[2] = new Reverb();
	effects[3] = new BitCrusher();
	for(int i = 0; i < (uint8_t)EffectData::Type::COUNT - 1; i++){
		effects[i]->setIntensity(config.effects[i].intensity);
	}
}

File EditSlot::openFile(SlotConfig config){
	File file = SPIFFS.open(String("/Samples/") + sampleNames[uint8_t(config.sample)] + ".aac");
	return file;
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
	speeder.setSpeed((float)speed /  255.0);
}

