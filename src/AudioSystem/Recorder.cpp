#include "Recorder.h"
#include <FS/RamFile.h>
#include <Loop/LoopManager.h>
#include "../Model/SlotConfig.h"
#include <Setup.hpp>
#include <Data/FileDataSource.h>
#include <Audio/SourceWAV.h>

static const char* TAG = "Recorder";

struct HeaderWAV {
	char RIFF[4];
	uint32_t chunkSize;
	char WAVE[4];
	char fmt[4];
	uint32_t fmtSize;
	uint16_t audioFormat;
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate; // == SampleRate * NumChannels * BitsPerSample/8
	uint16_t blockAlign; // == NumChannels * BitsPerSample/8
	uint16_t bitsPerSample;
	char data[4];
	uint32_t dataSize; // == NumSamples * NumChannels * BitsPerSample/8
};

//TODO - staviti pin defineove u lib
const i2s_pin_config_t pinconf = {
		.bck_io_num = 16,   // BCKL
		.ws_io_num = 27,    // LRCL
		.data_out_num = 4, // not used (only for speakers)
		.data_in_num = 32
};

const i2s_config_t recconfig = {
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


Recorder::Recorder(uint8_t slot) : slot(slot), i2s(recconfig, pinconf), task("recTask", [](Task* task){
	auto recorder = static_cast<Recorder*>(task->arg);
	recorder->recordFunc();
}, 8192, this){

	i2sBuffer = static_cast<char*>(malloc(i2sBufferSize));
	wavBuffer = static_cast<int16_t*>(malloc(wavBufferSize));
}

Recorder::~Recorder(){
	free(i2sBuffer);
	free(wavBuffer);
}

void Recorder::start(){
	if(state != WAITING) return;
	state = RECORDING;

	i2s.init();
	task.start(1, 0);
}

void Recorder::recordFunc(){
	while(task.running){
		if(state != RECORDING) return;

		size_t readSize;
		i2s_read(I2S_NUM_0, i2sBuffer, i2sBufferSize, &readSize, portMAX_DELAY);

		for(int j = 0; j < i2sBufferSize; j += 4){
			int16_t sample = *(int16_t*)(&i2sBuffer[j + 2]) + 3705;

			wavBuffer[sampleCount + j / 4] = sample;
			sampleCount++;

			if(sampleCount >= maxSamples) break;
		}

		if(sampleCount >= maxSamples){
			stop();
			return;
		}
	}
}

void Recorder::stop(){
	if(state != RECORDING) return;
	state = DONE;

	task.stop(true);
	i2s.deinit();
}

void Recorder::commit(){
	SlotConfig conf;
	conf.sample.sample = Sample::SampleType::RECORDING;
	conf.sample.fileIndex = slot;
	conf.slotIndex = slot;

	File file = openSample(conf, "w");
	ESP_LOGD(TAG, "Commiting to %s %ld samples", file.name(), sampleCount);
	String filename(file.name());
	file.close();

	SPIFFS.remove(filename);
	file = SPIFFS.open(filename, "w");

	writeHeaderWAV(file, sampleCount * BYTES_PER_SAMPLE);

	size_t totalWritten = 0;
	const size_t bufSize = 4096;
	uint8_t* buf = static_cast<uint8_t*>(malloc(bufSize));
	while(totalWritten < sampleCount * BYTES_PER_SAMPLE){
		size_t size = min(bufSize, sampleCount * BYTES_PER_SAMPLE - totalWritten);
		totalWritten += file.write(reinterpret_cast<const uint8_t*>(wavBuffer) + totalWritten, size);
	}
	free(buf);

	file.close();

	ESP_LOGD(TAG, "Committing content: written %ld bytes", totalWritten);
}

bool Recorder::isRecording(){
	return state == RECORDING;
}

bool Recorder::isRecorded(){
	return state == DONE;
}

void Recorder::writeHeaderWAV(File& file, size_t size){
	HeaderWAV header;
	memcpy(header.RIFF, "RIFF", 4);
	header.chunkSize = size + 36;
	memcpy(header.WAVE, "WAVE", 4);
	memcpy(header.fmt, "fmt ", 4);
	header.fmtSize = 16;
	header.audioFormat = 1; //PCM
	header.numChannels = NUM_CHANNELS; //2 channels
	header.sampleRate = SAMPLE_RATE;
	header.byteRate = SAMPLE_RATE * NUM_CHANNELS * BYTES_PER_SAMPLE;
	header.blockAlign = NUM_CHANNELS * BYTES_PER_SAMPLE;
	header.bitsPerSample = BYTES_PER_SAMPLE * 8;
	memcpy(header.data, "data", 4);
	header.dataSize = size;

	file.seek(0);
	size_t written = file.write(reinterpret_cast<const uint8_t*>(&header), sizeof(HeaderWAV));
	ESP_LOGD(TAG, "Committing header: written %ld bytes", written);
}

