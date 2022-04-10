#include <Synthia.h>
#include <SPIFFS.h>
#include "Intro.h"
#include "../AudioSystem/PlaybackSystem.h"
#include "../SaveManager.h"
#include <Devices/Matrix/MatrixAnimGIF.h>
#include <Loop/LoopManager.h>

static const char* TAG = "Intro";

// TODO: anims
const Intro::AnimMapping Intro::IntroAnims[5] = {
		{ Synthia.TrackMatrix, "/Intro/Track.gif" },
		{ Synthia.CursorMatrix, "/Intro/Cursor.gif" },
		{ Synthia.SlidersMatrix, "/Intro/Sliders.gif" },
		{ Synthia.TrackRGB, "/Intro/TrackRGB.gif" },
		{ Synthia.SlotRGB, "/Intro/SlotRGB.gif" }
};

void Intro::onStart(){
	for(int i = 0; i < 5; i++){
		auto file = SPIFFS.open(IntroAnims[i].filename);
		if(!file){
			ESP_LOGE(TAG, "Can't open intro file %s", IntroAnims[i].filename);
			continue;
		}

		anims[i] = std::unique_ptr<MatrixAnimGIF>(new MatrixAnimGIF(file));

		anims[i]->getGIF().setLoopMode(GIF::SINGLE);
		anims[i]->setMatrix(&IntroAnims[i].matrix);
		anims[i]->start();
	}

	// TODO: first time data

	std::array<SlotConfig, 5> configs;
	for(int i = 0; i < 5; i++){
		configs[i].sample.sample = (Sample::SampleType) i;
		configs[i].slotIndex = i;
	}
	configs[4].sample.sample = Sample::SampleType::KICK;
	configs[4].speed = 50;
	baker = std::unique_ptr<Baker>(new Baker(configs));

	baker->start();

	LoopManager::addListener(this);
}

void Intro::onStop(){
	if(!baker->isDone()){
		ESP_LOGE(TAG, "Intro stopped before baker done!");
	}

	for(int i = 0; i < 5; i++){
		anims[i]->stop();
	}

	loadingAnim->stop();

	LoopManager::removeListener(this);
}

void Intro::loop(uint micros){
	if(!introAnimDone){
		int doneAnims = 0;
		for(int i = 0; i < 5; i++){
			if(!anims[i]->isStarted()){
				doneAnims++;
			}
		}

		if(doneAnims == 5){
			if(baker->isDone()){
				// TODO: start track edit
				LoopManager::removeListener(this);
				return;
			}

			introAnimDone = true;

			for(int i = 0; i < 5; i++){
				anims[i]->stop();
			}

			// TODO: start loading anim
			auto file = SPIFFS.open("/Loading.gif");
			if(!file){
				ESP_LOGE(TAG, "Can't open loading file");
				return;
			}

			loadingAnim = std::unique_ptr<MatrixAnimGIF>(new MatrixAnimGIF(file));
			loadingAnim->getGIF().setLoopMode(GIF::INFINITE);
			loadingAnim->setMatrix(&Synthia.TrackMatrix);
			loadingAnim->start();
		}

		return;
	}

	if(!baker->isDone()) return;

	// TODO: start track edit
}
