#include "SlotConfig.h"

const char* sampleNames[] = {"kick", "snare", "clap", "closedhihat", "openhihat"};

File openSample(SlotConfig config, const char* mode){
	String path;
	if(config.sample.sample == Sample::SampleType::RECORDING){
		if(!SPIFFS.exists("/Recordings")){
			SPIFFS.mkdir("/Recordings");
		}
		path = String("/Recordings/") + config.sample.fileIndex + ".wav";
	}else{
		path = String("/Samples/") + sampleNames[uint8_t(config.sample.sample)] + ".wav";
	}
	return SPIFFS.open(path, mode);
}
