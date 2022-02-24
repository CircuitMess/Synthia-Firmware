#include "PlaybackSlot.h"

PlaybackSlot::PlaybackSlot(File file) : ds(file), source(ds){
}

Source &PlaybackSlot::getSource(){
	return source;
}
