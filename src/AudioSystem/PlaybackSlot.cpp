#include "PlaybackSlot.h"

PlaybackSlot::PlaybackSlot(File file) : ds(file), source(ds){
	source.seek(0, fs::SeekEnd);
}

Generator & PlaybackSlot::getGenerator(){
	return source;
}

void PlaybackSlot::seek(size_t pos, SeekMode mode){
	source.seek(pos, mode);
}

SourceWAV & PlaybackSlot::getSource(){
	return source;
}

PlaybackSlot::~PlaybackSlot(){

}
