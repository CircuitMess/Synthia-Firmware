#include <Synthia.h>
#include <SPIFFS.h>
#include "Intro.h"
#include "../AudioSystem/PlaybackSystem.h"
#include "../SaveManager.h"
#include "TrackEditState.h"
#include <Devices/Matrix/MatrixAnimGIF.h>
#include <Loop/LoopManager.h>

static const char* TAG = "Intro";

// TODO: anims
const Intro::AnimMapping Intro::IntroAnims[5] = {
		{ Synthia.TrackMatrix, "/GIF/Intro/Track.gif", false },
		{ Synthia.CursorMatrix, "/GIF/Intro/Cursor.gif", false },
		{ Synthia.SlidersMatrix, "/GIF/Intro/Sliders.gif", true },
		{ Synthia.TrackRGB, "/GIF/Intro/TrackRGB.gif", true },
		{ Synthia.SlotRGB, "/GIF/Intro/SlotRGB.gif", true }
};

static const i2s_config_t i2s_config = {
		.mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
		.sample_rate = SAMPLE_RATE,
		.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
		.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
		.communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
		.intr_alloc_flags = 0,
		.dma_buf_count = 2,
		.dma_buf_len = 512,
		.use_apll = false,
		.tx_desc_auto_clear = true,
		.fixed_mclk = 0
};

Intro::Intro() : audioFile(RamFile::open(SPIFFS.open("/Synthia.aac"))), fileSource(audioFile), source(fileSource), output(i2s_config, i2s_pin_config){
	output.setSource(&source);
}

void Intro::onStart(){
	for(int i = 0; i < 5; i++){
		auto file = SPIFFS.open(IntroAnims[i].filename);
		if(!file){
			ESP_LOGE(TAG, "Can't open intro file %s", IntroAnims[i].filename);
			continue;
		}

		file = RamFile::open(file);

		anims[i] = std::unique_ptr<MatrixAnimGIF>(new MatrixAnimGIF(file));

		anims[i]->getGIF().setLoopMode(IntroAnims[i].loop ? GIF::INFINITE : GIF::SINGLE);
		anims[i]->setMatrix(&IntroAnims[i].matrix);
	}

	for(int i = 0; i < 5; i++){
		anims[i]->start();
	}

	// TODO: first time data

	std::array<SlotConfig, 5> configs;
	for(int i = 0; i < 5; i++){
		configs[i].sample.sample = (Sample::SampleType) i;
		configs[i].slotIndex = i;
	}
	baker = std::unique_ptr<Baker>(new Baker(configs));

	output.setGain(0.3);
	output.start();

	LoopManager::addListener(this);
}

void Intro::onStop(){
	if(!baker->isDone()){
		ESP_LOGE(TAG, "Intro stopped before baker done!");
	}

	for(int i = 0; i < 5; i++){
		anims[i]->stop();
	}

	if(loadingAnim){
		loadingAnim->stop();
	}

	LoopManager::removeListener(this);
}

void Intro::loop(uint micros){
	if(output.isRunning()){
		output.loop(0);
	}else if(!baker->isBaking() && !baker->isDone()){
		baker->start();
	}

	if(!introAnimDone){
		if(!anims[0]->isStarted()){
			if(baker->isDone()){
				LoopManager::removeListener(this);
				launch();
				return;
			}

			introAnimDone = true;

			for(int i = 0; i < 5; i++){
				anims[i]->stop();
				if(&IntroAnims[i].matrix != &Synthia.TrackMatrix){
					IntroAnims[i].matrix.clear();
					IntroAnims[i].matrix.push();
				}
			}

			// TODO: add loading animation
			auto file = SPIFFS.open("/GIF/Loading.gif");
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

	LoopManager::removeListener(this);
	launch();
}

void Intro::launch(){
	stop();
	delete this;

	auto trackEdit = new TrackEditState();
	trackEdit->start();

	LoopManager::addListener(Synthia.getInput());
}
