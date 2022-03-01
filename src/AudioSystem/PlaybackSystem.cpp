#include "PlaybackSystem.h"
#include <SPIFFS.h>
#include <utility>
#include "../SlotManager.h"
//#include <Synthia.h>
#include <JayD.h>

#include <Loop/LoopManager.h>

const i2s_config_t config = {
		.mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_TX),
		.sample_rate = SAMPLE_RATE,
		.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
		.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
		.communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
		.intr_alloc_flags = 0,
		.dma_buf_count = 16,
		.dma_buf_len = 512,
		.use_apll = false,
		.tx_desc_auto_clear = true,
		.fixed_mclk = 0
};


PlaybackSystem::PlaybackSystem() : output(config, i2s_pin_config, I2S_NUM_0){
	output.setSource(&mixer);
//	output.setGain(0.2);
}

void PlaybackSystem::loop(uint micros){
	jobsMutex.lock();
	if(!jobs.empty()){
		auto job = jobs.front();
		processJob(job);
		jobs.pop();
	}
	jobsMutex.unlock();
	output.loop(0);
}

void PlaybackSystem::block(uint8_t slot){
	jobsMutex.lock();
	jobs.push({AudioJob::SET, slot, nullptr});
	jobsMutex.unlock();
}

void PlaybackSystem::init(){
	//TODO - create EditSlots with config from SlotManager, bake, init PlaybackSlots with RamFile from baking
	for(int i = 4; i >= 0; --i){
		SlotConfig conf;
		conf.sample = SlotConfig::SampleType(i);
		conf.slotIndex = i;

		File temp = openSample(conf);
		slots[i] = new PlaybackSlot(RamFile::open(temp));
		temp.close();
		mixer.addSource(&slots[i]->getGenerator());
	}
	LoopManager::addListener(this);
}

void PlaybackSystem::play(uint8_t slot){
	jobsMutex.lock();
	jobs.push({AudioJob::PLAY, slot, nullptr});
	jobsMutex.unlock();
}

void PlaybackSystem::set(uint8_t slot, File file){
	auto temp = new PlaybackSlot(std::move(file));
	jobsMutex.lock();
	jobs.push({AudioJob::SET, slot, temp});
	jobsMutex.unlock();
}

EditSlot PlaybackSystem::edit(uint8_t slot, SlotConfig config){
	auto temp = new EditSlot(config);
	jobsMutex.lock();
	jobs.push({AudioJob::SET, slot, temp});
	jobsMutex.unlock();
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
			delete slots[job.slot];
			slots[job.slot] = job.sampleSlot;
			break;
	}
}


