#include <cstdlib>
#include "ButtonActionSystem.h"
#include <Loop/LoopManager.h>
#include <Input/Input.h>

ButtonActionSystem ButtonActions;

void ButtonActionSystem::begin(){
	LoopManager::addListener(this);
	Input::getInstance()->addListener(this);
}

void ButtonActionSystem::loop(uint micros){
	if(clickTimes.empty()) return;
	for(auto it = clickTimes.begin(); it != clickTimes.end();){
		if(it->second + doubleClickInterval <= millis()){
			auto button = it->first;
			iterateListeners([&button](ButtonActionListener* listener){
				listener->singleClick(button);
			});

			it = clickTimes.erase(it);
		}else{
			it++;
		}
	}
}

void ButtonActionSystem::buttonPressed(uint i){
	if(!clickTimes.count(i)){
		clickTimes[i] = millis();
	}else{
		iterateListeners([&i](ButtonActionListener* listener){
			listener->doubleClick(i);
		});
		clickTimes.erase(i);
	}
}
