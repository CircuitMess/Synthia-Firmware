#ifndef SYNTHIA_FIRMWARE_SLOTPLAYER_H
#define SYNTHIA_FIRMWARE_SLOTPLAYER_H

#include <Input/InputJayD.h>
#include <cstdint>

class SlotPlayer : public JayDInputListener{
public:
	void start();
	void enable();
	void disable();
	void play(uint8_t slot); //blue pulse

private:
	bool enabled = true;
	void buttonPress(uint8_t id) override;

	void playOnInput(uint8_t slot); //green pulse
};

extern SlotPlayer slotPlayer;

#endif //SYNTHIA_FIRMWARE_SLOTPLAYER_H
