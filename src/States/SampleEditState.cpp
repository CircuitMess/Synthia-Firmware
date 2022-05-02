#include <Loop/LoopManager.h>
#include "SampleEditState.h"
#include "../Visualization/RGBController.h"
#include "../AudioSystem/PlaybackSystem.h"
#include "../AudioSystem/SlotBaker.h"
#include "../Visualization/LEDStrip.h"

SampleEditState::SampleEditState(State* parent, uint8_t slot) : State(parent), slot(slot), config(Playback.getConfig(slot)){
	config.sample.fileIndex = config.slotIndex = slot;

	load = new Task("SampleEdit-Load", [](Task* task){
		SampleEditState* state = static_cast<SampleEditState*>(task->arg);

		SlotConfig defaultConfigs[5];
		for(uint8_t i = 0; i < 5; i++){
			defaultConfigs[i].sample.type = (Sample::Type)i;
			defaultConfigs[i].slotIndex = defaultConfigs[i].sample.fileIndex = i;

			File file = openSample(defaultConfigs[i]);
			state->rawSamples[i] = RamFile::open(file);
			file.close();
		}

		if(state->editSlot){
			Playback.release(state->slot);
			while(Playback.getSlot(state->slot)){
				delay(1);
			}
		}

		if(state->config.sample.type == Sample::Type::RECORDING){
			File f = openSample(state->config);
			state->editSlot = new EditSlot(state->config, RamFile::open(f));
		}else{
			state->editSlot = new EditSlot(state->config, RamFile::open(state->rawSamples[((uint8_t)state->config.sample.type)]));
		}
	}, 2048, this);

	setButtonHoldTime(Synthia.slotToBtn(slot), 500);
}

SampleEditState::~SampleEditState(){
	delete editSlot;
	load->stop(true);
	delete load;
}

void SampleEditState::onStart(){
	RGBSlot.setColor(slot, MatrixPixel::Yellow);

	load->start(1, 0);
	MatrixAnimGIF intro(SPIFFS.open("/GIF/SampleEdit.gif"), &Synthia.TrackMatrix);
	intro.getGIF().setLoopMode(GIF::SINGLE);
	intro.start();
	while(load->running || intro.isStarted()){
		intro.loop(0);
	}


	Playback.edit(slot, editSlot);


	Encoders.addListener(this);
	Sliders.addListener(this);
	Input::getInstance()->addListener(this);

	sampleVis.setMain();
	sampleVis.push({ config.sample.type, true });

	LEDStrip.setLeftFromCenter((int8_t)((int)(config.speed) - 128));
	LEDStrip.setRight(config.effects[(size_t) selectedEffect].intensity);

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

	if(editSlot){
		Playback.release(slot);
		while(Playback.getSlot(slot)){
			delay(1);
		}
	}
}

void SampleEditState::buttonHeld(uint i){
	int s = Synthia.btnToSlot(i);
	if(s != slot) return;

	if(config.sample.type == Sample::Type::RECORDING && recorder && recorder->isRecording()){
		return;
	}

	// TODO: loading
	Synthia.TrackMatrix.clear();
	Synthia.TrackMatrix.push();
	LEDStrip.setMidFill(0);
	LEDStrip.setRight(0);
	LEDStrip.setLeft(0);

	Task bake("SampleEdit-Bake", [](Task* task){
		SampleEditState* state = static_cast<SampleEditState*>(task->arg);

		Playback.release(state->slot);
		while(Playback.getSlot(state->slot) != nullptr){
			delay(1);
		}

		File file = RamFile::create();
		SlotBaker baker(state->editSlot, file);
		baker.start();
		while(!baker.isDone()){
			delay(1);
		}

		Playback.set(state->slot, file, state->config);
	}, 2048, this);
	bake.start(1, 0);

	MatrixAnimGIF outro(SPIFFS.open("/GIF/TrackEdit.gif"), &Synthia.TrackMatrix);
	outro.getGIF().setLoopMode(GIF::SINGLE);
	outro.start();
	while(bake.running || outro.isStarted()){
		outro.loop(0);
	}

	delete editSlot;
	editSlot = nullptr;

	pop();
}

void SampleEditState::buttonPressed(uint i){
	int s = Synthia.btnToSlot(i);
	if(s != slot) return;
	if(config.sample.type != Sample::Type::RECORDING) return;
	if(recorder == nullptr || recorder->isRecording()) return;

	RGBSlot.setSolid(slot, MatrixPixel::Red);

	recorder->start();
	LoopManager::addListener(this);
}

void SampleEditState::buttonReleased(uint i){
	int s = Synthia.btnToSlot(i);
	if(s != slot) return;
	if(config.sample.type != Sample::Type::RECORDING) return;
	if(!recorder || !recorder->isRecording()) return;

	recorder->stop();
}

void SampleEditState::leftEncMove(int8_t amount){
	int8_t type = static_cast<int8_t>(config.sample.type);

	if(config.sample.type == Sample::Type::RECORDING && recorder && recorder->isRecording()){
		return;
	}

	if(!sampleVis.isStarted()){
		sampleVis.push({ (Sample::Type) type, recorder == nullptr });
		Playback.play(slot);
		return;
	}

	type += amount;
	if(type < 0 || type >= (uint8_t) Sample::Type::SIZE) return;

	if(recorder){
		if(recorder->isRecording()){
			recorder->stop();
		}

		delete recorder;
		recorder = nullptr;
	}

	config.sample.type = (Sample::Type) type;
	if(config.sample.type == Sample::Type::RECORDING){
		editSlot = nullptr;
		Playback.block(slot);

		recorder = new Recorder();
		RGBSlot.blinkContinuous(slot, MatrixPixel::Red);
	}else{
		editSlot = new EditSlot(config, RamFile::open(rawSamples[type]));
		Playback.edit(slot, editSlot);
		Playback.play(slot);
		RGBSlot.setSolid(slot, MatrixPixel::Yellow);
	}

	sampleVis.push({ (Sample::Type) type, recorder == nullptr });
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
	config.speed = value;
	speedVis.push(value);

	if(!editSlot) return;
	editSlot->setSpeed(value);
}

void SampleEditState::rightPotMove(uint8_t value){
	config.effects[(size_t) selectedEffect].intensity = value;
	effectVis.push(config.effects[(uint8_t) selectedEffect]);

	if(!editSlot) return;
	editSlot->setEffect(selectedEffect, value);
}

void SampleEditState::loop(uint micros){
	if(!recorder){
		LoopManager::removeListener(this);
		return;
	}

	if(recorder->isRecording()){
		LEDStrip.setMidFill(recorder->getProgress() * 255.0f);
	}

	if(!recorder->isRecorded()) return;

	File f = RamFile::create();
	recorder->commit(f);
	delete recorder;
	recorder = nullptr;

	String path = String("/Recordings/") + config.sample.fileIndex + ".wav";
	SPIFFS.remove(path);
	File dest = SPIFFS.open(path, FILE_WRITE);

	f.seek(0);
	uint8_t* buf = static_cast<uint8_t*>(malloc(1024));
	size_t total = 0;
	while(total < f.size()){
		size_t size = f.read(buf, min(1024, f.size() - total));
		dest.write(buf, size);
		total += size;
	}
	dest.close();
	free(buf);

	editSlot = new EditSlot(config, f);
	Playback.edit(slot, editSlot);

	RGBSlot.setSolid(slot, MatrixPixel::Yellow);
	LEDStrip.setMidFill(0);
	LEDStrip.setLeftFromCenter(0);

	sampleVis.push({ config.sample.type, true });

	LoopManager::removeListener(this);
}