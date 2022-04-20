#include "SampleEditState.h"
#include "../Visualization/RGBController.h"
#include "../AudioSystem/PlaybackSystem.h"
#include "../AudioSystem/SlotBaker.h"
#include "../Visualization/LEDStrip.h"

SampleEditState::SampleEditState(State* parent, uint8_t slot) : State(parent), slot(slot), config(Playback.getConfig(slot)){

	//TODO - staviti otvaranje rawSampleova u drugi thread, loading animacija?
	SlotConfig defaultConfigs[5];
	// // editSlot = new EditSlot(config, RamFile::open(rawSamples[((uint8_t)config.sample.type)]));
	for(uint8_t i = 0; i < 5; i++){
		defaultConfigs[i].sample.type = (Sample::Type)i;
		defaultConfigs[i].slotIndex = i;

		File file = openSample(defaultConfigs[i]);
		rawSamples[i] = RamFile::open(file);
		file.close();
	}
	editSlot = new EditSlot(config, rawSamples[((uint8_t)config.sample.type)]);
	Playback.edit(slot, editSlot);

	setButtonHoldTime(Synthia.slotToBtn(slot), 500);
}

SampleEditState::~SampleEditState(){
	delete editSlot;
}

void SampleEditState::onStart(){
	Encoders.addListener(this);
	Sliders.addListener(this);
	Input::getInstance()->addListener(this);

	sampleVis.setMain();
	sampleVis.push(config.sample.type);
	RGBSlot.setColor(slot, MatrixPixel::Yellow);

	LEDStrip.setRightFromCenter(config.speed);

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

	int s = Synthia.btnToSlot(i);
	if(s != slot) return;

	Playback.block(slot);
	Synthia.TrackMatrix.clear();
	Synthia.TrackMatrix.push();
	delay(1000); // TODO: deal with the race condition properly. can proceed only after Playback releases EditSlot
	File file = RamFile::create();
	SlotBaker baker(editSlot, file);
	baker.start();
	while(!baker.isDone()) delay(1);
	Playback.set(slot, file, config);

	pop();
}

void SampleEditState::leftEncMove(int8_t amount){
	int8_t type = static_cast<int8_t>(config.sample.type);

	if(!sampleVis.isStarted()){
		sampleVis.push((Sample::Type) type);
		if((Sample::Type) type != Sample::Type::RECORDING){
			Playback.play(slot);
		}
		return;
	}

	type += amount;
	if(type < 0 || type >= (uint8_t) Sample::Type::SIZE) return;

	config.sample.type = (Sample::Type) type;
	if(config.sample.type != Sample::Type::RECORDING){
		editSlot = new EditSlot(config, RamFile::open(rawSamples[type]));
		Playback.edit(slot, editSlot);
	}
	sampleVis.push((Sample::Type) type);

	if((Sample::Type) type != Sample::Type::RECORDING){
		Playback.play(slot);
	}

	// TODO - go to recording state
}

void SampleEditState::rightEncMove(int8_t amount){
	int8_t effect = (int8_t) selectedEffect;

	if(!effectVis.isStarted()){
		effectVis.push({ selectedEffect, config.effects[effect].intensity });
		return;
	}

	effect += amount;
	if(effect < 0 || effect >= (int8_t) EffectData::Type::COUNT) return;

	selectedEffect = (EffectData::Type) effect;

	effectVis.push({ selectedEffect, config.effects[effect].intensity });
}

void SampleEditState::leftPotMove(uint8_t value){
	editSlot->setSpeed(value);
	speedVis.push(value);
}

void SampleEditState::rightPotMove(uint8_t value){
	editSlot->setEffect(selectedEffect, value);
	config.effects[(size_t) selectedEffect].intensity = value;
	effectVis.push(config.effects[(uint8_t) selectedEffect]);
}
