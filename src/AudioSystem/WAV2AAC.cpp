#include "WAV2AAC.h"

#include <utility>
#include <Data/FileDataSource.h>

WAV2AAC::WAV2AAC(File in, File out) : in(std::move(in)), out(std::move(out)), dataSource(WAV2AAC::in), src(dataSource), dst(WAV2AAC::out),
									  task("WAV2AAC", [](Task* task){ static_cast<WAV2AAC*>(task->arg)->transcode(); }, 8192, this){

	dst.setSource(&src);
}

void WAV2AAC::start(){
	if(state != WAITING) return;
	state = WORKING;
	task.start(1, 0);
}

bool WAV2AAC::isDone(){
	return state == DONE;
}

void WAV2AAC::transcode(){
	dst.start();
	while(dst.isRunning()){
		dst.loop(0);
	}
	out.seek(0);
	state = DONE;
}
