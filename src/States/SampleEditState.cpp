#include "SampleEditState.h"
#include "../Visualization/RGBController.h"
#include "../AudioSystem/PlaybackSystem.h"
#include "../AudioSystem/SlotBaker.h"

SampleEditState::SampleEditState(State* state, uint8_t slot) : State(state), slot(slot), config(Playback.getConfig(slot)){

	//TODO - staviti otvaranje rawSampleova u drugi thread, loading animacija?
	SlotConfig defaultConfigs[5];
	for(uint8_t i = 0; i < 5; i++){
		defaultConfigs[i].sample.sample = (Sample::SampleType)i;
		defaultConfigs[i].slotIndex = i;

		File file = openSample(defaultConfigs[i]);
		rawSamples[i] = RamFile::open(file);
		file.close();
	}
	editSlot = new EditSlot(config, rawSamples[((uint8_t)config.sample.sample)]);
	Playback.edit(slot, editSlot);
}

void SampleEditState::onStart(){
	Encoders.addListener(this);
	Sliders.addListener(this);
	Input::getInstance()->addListener(this);

	setButtonHoldTime(slot, 800);

	sampleVis.setMain();
	sampleVis.push(config.sample.sample);
	RGBSlot.setColor(slot, MatrixPixel::Yellow);

	//TODO - start rgb anim for track leds
	//Synthia.TrackRGB.startAnimation()
}

void SampleEditState::onStop(){
	Encoders.removeListener(this);
	Sliders.removeListener(this);
	Input::getInstance()->removeListener(this);

	RGBSlot.clear();
	//TODO - stop rgb anim for track leds

	VMan.clearMain();
}

bool first = true;

void SampleEditState::buttonHeld(uint i){
	if(first){
		first = false;
		return;
	}
	if(i != slot)return;

	File file = RamFile::create();
	SlotBaker baker(editSlot, file);
	baker.start();
	while(!baker.isDone()) delay(1);
	Playback.set(slot, file, config);

	pop();
}

void SampleEditState::leftEncMove(int8_t amount){
	int8_t sampleType = static_cast<int8_t>(config.sample.sample);
	printf("left enc move amount: %d\n", amount);
	//no wrapping allowed

	sampleType += amount;
	if(sampleType < 0 || sampleType >= (int8_t) Sample::SampleType::SIZE) return;

	printf("sample: %d\n", sampleType);
	editSlot->setSample((Sample::SampleType) sampleType, rawSamples[sampleType]);
	Playback.play(slot);
	sampleVis.push((Sample::SampleType) sampleType);

	/*if(sampleType == Sample::SampleType::RECORDING){
		//TODO - go to recording state
	}*/
}

void SampleEditState::rightEncMove(int8_t amount){
	//no wrapping allowed
	if((amount < 0 && ((uint8_t)selectedEffect) == 0) || (amount > 0 && selectedEffect == EffectData::Type::COUNT)) return;

	selectedEffect = (EffectData::Type)(((uint8_t)selectedEffect) + amount);
	effectVis.push(config.effects[(uint8_t)selectedEffect]);
	Serial.printf("effect: %d\n", (uint8_t)selectedEffect);

}

void SampleEditState::leftPotMove(uint8_t value){
	editSlot->setSpeed(value);
	speedVis.push(value);
}

void SampleEditState::rightPotMove(uint8_t value){
	editSlot->setEffect(selectedEffect, value);
	effectVis.push(config.effects[(uint8_t)selectedEffect]);
}
