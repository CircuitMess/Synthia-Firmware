#include "SlotConfig.h"

const char* sampleNames[] = {"kick", "snare", "clap", "closedhihat", "openhihat"};

File openSample(SlotConfig config){
	String path;
	if(config.sample == SlotConfig::SampleType::RECORDING){
		path = String("/Recordings/") + config.slotIndex + ".wav";
	}else{
		path = String("/Samples/") + sampleNames[uint8_t(config.sample)] + ".wav";
	}
	return SPIFFS.open(path);
}
