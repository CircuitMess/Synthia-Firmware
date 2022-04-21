#ifndef SYNTHIA_FIRMWARE_SLOTPLAYER_H
#define SYNTHIA_FIRMWARE_SLOTPLAYER_H

#include <Input/InputListener.h>

class SlotPlayer : public InputListener {
public:
	void begin();
	void enable();
	void disable();
	void play(uint8_t slot); //blue pulse

private:
	bool enabled = false;
	void buttonPressed(uint id) override;

	void playOnInput(uint8_t slot); //green pulse
};

extern SlotPlayer Player;

#endif //SYNTHIA_FIRMWARE_SLOTPLAYER_H
