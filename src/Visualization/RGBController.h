#ifndef SYNTHIA_FIRMWARE_RGBCONTROLLER_H
#define SYNTHIA_FIRMWARE_RGBCONTROLLER_H

#include <Loop/LoopListener.h>
#include <Devices/Matrix/MatrixPixel.h>

class Matrix;

class RGBController : public LoopListener {
public:
	RGBController();
	void begin(Matrix* matrix);

	void setColor(uint8_t slot, MatrixPixel color = { 0, 0, 0, 0 });
	void clear();
	void blink(uint8_t slot, MatrixPixel color);
	void blinkAll(MatrixPixel color);
	void blinkTwice(uint8_t slot, MatrixPixel color);
	void blinkAllTwice(MatrixPixel color);
	void blinkContinuous(uint8_t slot, MatrixPixel color);
	void playAnim();

	void loop(uint micros) final;

private:
	Matrix* matrix = nullptr;

	enum {
		Single, Anim
	} state = Single;

	enum {
		Solid, Once, Twice, Continuous
	} slotState[5] = {Solid, Solid, Solid, Solid, Solid};

	MatrixPixel slotColors[5] = {{0, 0, 0, 0},
								 {0, 0, 0, 0},
								 {0, 0, 0, 0},
								 {0, 0, 0, 0},
								 {0, 0, 0, 0}};
	MatrixPixel blinkColors[5];
	uint32_t blinkMicros[5] = {0};
	bool blinkState[5] = {false, false, false, false, false};
	const uint32_t blinkTime = 200; //[ms]

	bool pushNeeded = false;
};

extern RGBController RGBTrack;
extern RGBController RGBSlot;

#endif //SYNTHIA_FIRMWARE_RGBCONTROLLER_H
