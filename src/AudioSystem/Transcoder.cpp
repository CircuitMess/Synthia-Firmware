#include "Transcoder.h"

#include <utility>
#include <Data/FileDataSource.h>
#include <Audio/SourceAAC.h>
#include <Audio/OutputWAV.h>
#include <Audio/SourceWAV.h>
#include <Audio/OutputAAC.h>

Transcoder::Transcoder(Type type, File in, File out) : in(std::move(in)), out(std::move(out)), dataSource(Transcoder::in), type(type),
											task("WAV2AAC", [](Task* task){ static_cast<Transcoder*>(task->arg)->transcode(); }, 8192, this){

	switch(type){
		case WAV2AAC:
			src = new SourceWAV(dataSource);
			dst = new OutputAAC(this->out);
			break;
		case AAC2WAV:
			src = new SourceAAC(dataSource);
			dst = new OutputWAV(this->out);
			break;
		case CUSTOM:
			return;
	}

	dst->setSource(src);
}

Transcoder::~Transcoder(){
	delete src;
	delete dst;
}

void Transcoder::setCoders(Source* source, Output* output){
	if(type != CUSTOM) return;
	src = source;
	dst = output;
	dst->setSource(src);
}

void Transcoder::start(){
	if(state != WAITING) return;

	if(type == CUSTOM && (src == nullptr || dst == nullptr)) return;

	state = WORKING;
	task.start(1, 0);
}

bool Transcoder::isDone(){
	return state == DONE;
}

void Transcoder::transcode(){
	dst->start();
	while(dst->isRunning()){
		dst->loop(0);
	}
	out.seek(0);
	state = DONE;
}
