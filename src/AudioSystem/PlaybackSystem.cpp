#include "PlaybackSystem.h"
#include <SPIFFS.h>
#include "../SlotManager.h"
#include <Synthia.h>

const i2s_config_t config = {
		.mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_TX),
		.sample_rate = SAMPLE_RATE,
		.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
		.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
		.communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
		.intr_alloc_flags = 0,
		.dma_buf_count = 2,
		.dma_buf_len = 512,
		.use_apll = false,
		.tx_desc_auto_clear = true,
		.fixed_mclk = 0
};


PlaybackSystem::PlaybackSystem() : output(config, i2s_pin_config, I2S_NUM_0), jobs(15, sizeof(AudioJob)), task("Playback", PlaybackSystem::taskFunc, 4096, this){
	output.setSource(&mixer);
}

void PlaybackSystem::init(){
	if(task.running) return;

	//TODO - create EditSlots with config from SlotManager, bake, init PlaybackSlots with RamFile from baking
	for(int i = 0; i < 5; ++i){
		SlotConfig conf;
		conf.sample.sample = Sample::SampleType(i);
		conf.sample.fileIndex = i;
		conf.slotIndex = i;

		File temp = openSample(conf);
		slots[i] = new PlaybackSlot(RamFile::open(temp));
		temp.close();
		mixer.addSource(&slots[i]->getGenerator());
	}

	task.start(0, 0);
}

void PlaybackSystem::block(uint8_t slot){
	jobs.send(new AudioJob{AudioJob::SET, slot, nullptr});
}

void PlaybackSystem::play(uint8_t slot){
	jobs.send(new AudioJob{AudioJob::PLAY, slot, nullptr});
}

void PlaybackSystem::set(uint8_t slot, File file){
	auto temp = new PlaybackSlot(std::move(file));
	jobs.send(new AudioJob{AudioJob::SET, slot, temp});
}

EditSlot* PlaybackSystem::edit(uint8_t slot, const SlotConfig& config){
	auto temp = new EditSlot(config);
	jobs.send(new AudioJob{AudioJob::SET, slot, temp});
	return temp;
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
			delete slots[job.slot];
			mixer.setSource(job.slot, &job.sampleSlot->getGenerator());
			slots[job.slot] = job.sampleSlot;
			break;
	}
}
