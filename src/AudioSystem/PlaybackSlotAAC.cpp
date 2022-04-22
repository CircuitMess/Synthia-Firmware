#include "PlaybackSlotAAC.h"

PlaybackSlotAAC::PlaybackSlotAAC(File file) : ds(file), source(ds){
	source.seek(0, fs::SeekEnd);
}

PlaybackSlotAAC::~PlaybackSlotAAC(){

}

Generator& PlaybackSlotAAC::getGenerator(){
	return source;
}

SourceAAC& PlaybackSlotAAC::getSource(){
	return source;
}

void PlaybackSlotAAC::seek(size_t pos, SeekMode mode){
	source.seek(pos, mode);
}
