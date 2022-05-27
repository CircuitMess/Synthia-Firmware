#include "SimplePlaybackSystem.h"
#include <SPIFFS.h>
#include "../SaveManager.h"
#include <Synthia.h>

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


SimplePlaybackSystem::SimplePlaybackSystem() : task("Playback", [](Task* task){
	auto system = static_cast<SimplePlaybackSystem*>(task->arg);
	system->player();
}, 4096, this), output(i2s_config, i2s_pin_config, I2S_NUM_0), playQueue(4, sizeof(PlayJob)){

	output.setSource(source);
}

SimplePlaybackSystem::~SimplePlaybackSystem(){
	stop();
}

void SimplePlaybackSystem::begin(){
	if(task.running) return;

	task.start(1, 0);
}

void SimplePlaybackSystem::stop(){
	task.stop(true);
	delete source;
	delete dataSource;
}

void SimplePlaybackSystem::play(File file){
	FileDataSource* dataSource = new FileDataSource(file);
	SourceAAC* source = new SourceAAC(*dataSource);

	PlayJob job = { dataSource, source };
	playQueue.send(&job);

	running = true;
}

void SimplePlaybackSystem::player(){
	while(task.running){
		while(playQueue.count()){
			PlayJob next;
			if(!playQueue.receive(&next)) continue;

			if(source){
				source->close();
			}

			delete source;
			delete dataSource;

			dataSource = next.dataSource;
			source = next.source;

			output.setSource(source);
			output.start();
		}

		bool wasRunning = output.isRunning();
		output.loop(0);
		if(wasRunning && !output.isRunning()){
			running = false;
		}
	}
}

void SimplePlaybackSystem::setVolume(uint8_t volume){
	output.setGain((float) volume / 255.0f);
}

bool SimplePlaybackSystem::isPlaying(){
	return running;
}
