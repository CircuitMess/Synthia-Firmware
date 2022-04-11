#ifndef SYNTHIA_FIRMWARE_BUTTONACTIONSYSTEM_H
#define SYNTHIA_FIRMWARE_BUTTONACTIONSYSTEM_H

#include <WithListeners.h>
#include <Input/Input.h>
#include <cstdlib>
#include <unordered_map>

class ButtonActionSystem;

class ButtonActionListener {
	friend ButtonActionSystem;
private:
	virtual void singleClick(uint i){};
	virtual void doubleClick(uint i){};
};

class ButtonActionSystem : public WithListeners<ButtonActionListener>, public InputListener, public LoopListener {
public:
	void begin();
	void loop(uint micros) override;

private:
	void buttonPressed(uint i) override;

	const uint32_t doubleClickInterval = 500; //[ms]
	std::unordered_map<uint, ulong> clickTimes;
};

extern ButtonActionSystem ButtonActions;

#endif //SYNTHIA_FIRMWARE_BUTTONACTIONSYSTEM_H
