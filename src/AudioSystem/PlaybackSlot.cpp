#include "PlaybackSlot.h"

PlaybackSlot::PlaybackSlot(File file) : ds(file), source(ds){
	source.seek(0, fs::SeekEnd);
}

PlaybackSlot::~PlaybackSlot(){

}

Generator& PlaybackSlot::getGenerator(){
	return source;
}

SourceWAV& PlaybackSlot::getSource(){
	return source;
}

void PlaybackSlot::seek(size_t pos, SeekMode mode){
	source.seek(pos, mode);
}