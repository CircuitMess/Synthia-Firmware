#include "Recorder.h"
#include <FS/RamFile.h>
#include <Loop/LoopManager.h>
#include <Setup.hpp>
#include <Synthia.h>

static const char* TAG = "Recorder";

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

Recorder::Recorder() : task("Recorder", [](Task* task){
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

	sampleCount = 0;
	task.start(1, 0);
}

void Recorder::recordFunc(){
	esp_err_t err;
	if((err = i2s_driver_install(I2S_NUM_1, &recconfig, 0, NULL)) != ESP_OK){
		ESP_LOGE(TAG, "Failed installing I2S driver: %d ", err);

		state = WAITING;
		return;
	}
	if((err = i2s_set_pin(I2S_NUM_1, &i2s_pin_config)) != ESP_OK){
		ESP_LOGE(TAG, "Failed setting I2S pin: %d ", err);

		if((err = i2s_driver_uninstall(I2S_NUM_1)) != ESP_OK){
			ESP_LOGE(TAG, "Failed uninstalling I2S driver: %d ", err);
		}

		state = WAITING;
		return;
	}

	while(task.running){
		if(state != RECORDING) break;

		size_t readSize;
		i2s_read(I2S_NUM_1, i2sBuffer, i2sBufferSize, &readSize, portMAX_DELAY);

		for(int j = 0; j < i2sBufferSize; j += 4){
			int16_t sample = *(int16_t*)(&i2sBuffer[j + 2]);
			sample *= 2;

			if(sampleCount <= BUFFER_SAMPLES * 2){
				sample = (float) sample * pow((float) sampleCount / ((float) BUFFER_SAMPLES * 2.0f), 2);
			}

			wavBuffer[sampleCount] = sample;
			sampleCount++;

			if(sampleCount >= maxSamples) break;
		}

		if(sampleCount >= maxSamples) break;
	}

	if((err = i2s_driver_uninstall(I2S_NUM_1)) != ESP_OK){
		ESP_LOGE(TAG, "Failed installing I2S driver: %d ", err);
	}

	state = DONE;
}

void Recorder::stop(){
	if(state != RECORDING) return;
	task.stop(true);
}

void Recorder::commit(File& file){
	writeHeaderWAV(file, sampleCount * BYTES_PER_SAMPLE);

	size_t totalWritten = 0;
	while(totalWritten < sampleCount * BYTES_PER_SAMPLE){
		size_t size = min((size_t) 512, sampleCount * BYTES_PER_SAMPLE - totalWritten);
		totalWritten += file.write(reinterpret_cast<const uint8_t*>(wavBuffer) + totalWritten, size);
	}

	file.seek(0);

	ESP_LOGD(TAG, "Committing content: written %ld bytes", totalWritten);
}

bool Recorder::isRecording(){
	return state == RECORDING;
}

bool Recorder::isRecorded(){
	return state == DONE;
}

float Recorder::getProgress(){
	return (float) sampleCount / (float) maxSamples;
}

void Recorder::writeHeaderWAV(File& file, size_t size){
	struct HeaderWAV {
		char RIFF[4] = { 'R', 'I', 'F', 'F' };
		uint32_t chunkSize = 36;
		char WAVE[4] = { 'W', 'A', 'V', 'E' };
		char fmt[4] = { 'f', 'm', 't', ' ' };
		uint32_t fmtSize = 16;
		uint16_t audioFormat = 1; // PCM
		uint16_t numChannels = NUM_CHANNELS;
		uint32_t sampleRate = SAMPLE_RATE;
		uint32_t byteRate = SAMPLE_RATE * NUM_CHANNELS * BYTES_PER_SAMPLE;
		uint16_t blockAlign = NUM_CHANNELS * BYTES_PER_SAMPLE;
		uint16_t bitsPerSample = BYTES_PER_SAMPLE * 8;
		char data[4] = { 'd', 'a', 't', 'a' };
		uint32_t dataSize = 0;
	};

	HeaderWAV header;
	header.chunkSize += size;
	header.dataSize = size;

	file.seek(0);
	size_t written = file.write(reinterpret_cast<const uint8_t*>(&header), sizeof(HeaderWAV));
	ESP_LOGD(TAG, "Committing header: written %ld bytes", written);
}

