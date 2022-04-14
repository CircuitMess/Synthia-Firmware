#include "PlaybackSystem.h"
#include <SPIFFS.h>
#include "../SaveManager.h"
#include <Synthia.h>

PlaybackSystem Playback;

const i2s_config_t i2s_config = {
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


PlaybackSystem::PlaybackSystem() : output(i2s_config, i2s_pin_config, I2S_NUM_0), jobs(15, sizeof(AudioJob)), task("Playback", PlaybackSystem::taskFunc, 4096, this){
	output.setSource(&mixer);
	for(int i = 0; i < 5; ++i){
		mixer.addSource(nullptr);
	}
}

void PlaybackSystem::begin(){
	if(task.running) return;

	setVolume(Sliders.getRightPotValue());

	task.start(1, 0);
}

void PlaybackSystem::block(uint8_t slot){
	if(!task.running) return;

	AudioJob job { AudioJob::SET, slot, nullptr };
	jobs.send(&job);
}

void PlaybackSystem::play(uint8_t slot){
	if(!task.running) return;

	AudioJob job { AudioJob::PLAY, slot, nullptr };
	jobs.send(&job);
}

void PlaybackSystem::set(uint8_t slot, File file, const SlotConfig& config){
	if(!task.running) return;

	auto temp = new PlaybackSlot(file);
	configs[slot] = config;
	AudioJob job { AudioJob::SET, slot, temp };
	jobs.send(&job);
}

void PlaybackSystem::edit(uint8_t slot, EditSlot* editSlot){
	if(!task.running) return;

	AudioJob job { AudioJob::SET, slot, editSlot };
	jobs.send(&job);
}

void PlaybackSystem::taskFunc(Task* task){
	auto* system = static_cast<PlaybackSystem*>(task->arg);

	while(task->running){
		if(system->jobs.count()){
			AudioJob job;
			if(!system->jobs.receive(&job)) continue;
			system->processJob(job);
		}

		system->output.loop(0);

		vTaskDelay(1);
	}
}

void PlaybackSystem::processJob(AudioJob &job){
	switch(job.type){
		case AudioJob::PLAY:
			if(slots[job.slot] != nullptr){
				slots[job.slot]->play();
			}
			output.start();
			break;
		case AudioJob::SET:
			// TODO: only block should set to nullptr. used to remove the slot without deleting it
			if(job.sampleSlot != nullptr){
				delete slots[job.slot];
			}
			mixer.setSource(job.slot, job.sampleSlot ? &job.sampleSlot->getGenerator() : nullptr);
			slots[job.slot] = job.sampleSlot;
			break;
	}
}

const SlotConfig& PlaybackSystem::getConfig(uint8_t slot){
	return configs[slot];
}

uint8_t PlaybackSystem::getVolume() const{
	return volume;
}

void PlaybackSystem::setVolume(uint8_t volume){
	PlaybackSystem::volume = volume;
	float gain = volume / 255.0; //TODO - add function for volume level curve

	output.setGain(gain);
}
