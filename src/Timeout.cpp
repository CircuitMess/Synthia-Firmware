#include <Loop/LoopManager.h>

#include <utility>
#include "Timeout.h"

Timeout::Timeout(uint32_t time, std::function<void()>  callback) : time(time), callback(std::move(callback)){
	start = millis();
	LoopManager::addListener(this);
}

Timeout::~Timeout(){
	LoopManager::removeListener(this);
}

void Timeout::loop(uint micros){
	if(millis() < start + time) return;
	callback();
	delete this;
}
