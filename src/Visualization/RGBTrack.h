#ifndef SYNTHIA_FIRMWARE_RGBTRACK_H
#define SYNTHIA_FIRMWARE_RGBTRACK_H

#include <Loop/LoopListener.h>
#include <Devices/Matrix/MatrixPixel.h>

class RGBTrack : public LoopListener {
public:
	void setColor(uint8_t slot, MatrixPixel color);
	void clear();
	void blink(uint8_t slot, MatrixPixel color);
	void blinkAll(MatrixPixel color);
	void blinkTwice(uint8_t slot, MatrixPixel color);
	void blinkAllTwice(MatrixPixel color);
	void playRunning(MatrixPixel color);

	void loop(uint micros) override;

private:
	enum {
		Single, Anim
	} state = Single;

	enum {
		Solid, Once, Twice
	} slotState[5] = {Solid, Solid, Solid, Solid, Solid};

	MatrixPixel slotColors[5] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
	MatrixPixel blinkColors[5];
	uint32_t blinkMicros[5] = {0};
	bool blinkState[5] = {false, false, false, false, false};
	const uint32_t blinkTime = 350; //[ms]
};

#endif //SYNTHIA_FIRMWARE_RGBTRACK_H
