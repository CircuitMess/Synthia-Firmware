#include "PlaybackSystem.h"
#include <SPIFFS.h>
#include <utility>
#include "../SlotManager.h"
//#include <Synthia.h>

#include <Loop/LoopManager.h>

const i2s_pin_config_t i2s_pin_config = {
		.bck_io_num = 16,   // BCKL
		.ws_io_num = 27,    // LRCL
		.data_out_num = 4, // not used (only for speakers)
		.data_in_num = 32
};



const i2s_config_t config = {
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


PlaybackSystem::PlaybackSystem() : output(config, i2s_pin_config, I2S_NUM_0), jobs(15, sizeof(AudioJob)){
	output.setSource(&mixer);
//	output.setGain(0.2);
}

void PlaybackSystem::loop(uint micros){
	if(jobs.count()){
		AudioJob job;
		jobs.receive(&job);
		processJob(job);
	}
	output.loop(0);
}

void PlaybackSystem::block(uint8_t slot){
	jobs.send(new AudioJob{AudioJob::SET, slot, nullptr});
}

void PlaybackSystem::init(){
	//TODO - create EditSlots with config from SlotManager, bake, init PlaybackSlots with RamFile from baking
	for(int i = 0; i < 5; ++i){
		SlotConfig conf;
		conf.sample.sample = Sample::SampleType(i);
		conf.sample.fileIndex = i;
		conf.slotIndex = i;


		//not loading slot samples into ram because synthia doesnt have psram
//		File temp = openSample(conf);
		slots[i] = new PlaybackSlot(openSample(conf));
//		slots[i] = new PlaybackSlot(RamFile::open(temp));
//		temp.close();

		mixer.addSource(&slots[i]->getGenerator());
	}
	LoopManager::addListener(this);
}

void PlaybackSystem::play(uint8_t slot){
	jobs.send(new AudioJob{AudioJob::PLAY, slot, nullptr});
}

void PlaybackSystem::set(uint8_t slot, File file){
	auto temp = new PlaybackSlot(file);
	jobs.send(new AudioJob{AudioJob::SET, slot, temp});
}

EditSlot* PlaybackSystem::edit(uint8_t slot, SlotConfig config){
	auto temp = new EditSlot(config);
	jobs.send(new AudioJob{AudioJob::SET, slot, temp});
	return temp;
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
			if(job.sampleSlot == nullptr){
				mixer.pauseChannel(job.slot);
				if(slots[job.slot] != nullptr){
					delete slots[job.slot];
					slots[job.slot] = nullptr;
				}
				return;
			}

			if(slots[job.slot] != nullptr) delete slots[job.slot];
			mixer.resumeChannel(job.slot);
			mixer.setSource(job.slot, &job.sampleSlot->getGenerator());
			slots[job.slot] = job.sampleSlot;
			break;
	}
}


