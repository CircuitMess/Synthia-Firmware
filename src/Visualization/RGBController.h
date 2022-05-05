#ifndef SYNTHIA_FIRMWARE_RGBCONTROLLER_H
#define SYNTHIA_FIRMWARE_RGBCONTROLLER_H

#include <Loop/LoopListener.h>
#include <Devices/Matrix/MatrixPixel.h>
#include <Devices/Matrix/MatrixAnimGIF.h>

class Matrix;

class RGBController : public LoopListener {
public:
	RGBController();
	void begin(Matrix* matrix);

	void setColor(uint8_t slot, MatrixPixel color = { 0, 0, 0, 0 });
	void clear();
	void setSolid(uint8_t slot, MatrixPixel color);
	void blink(uint8_t slot, MatrixPixel color);
	void blinkAll(MatrixPixel color);
	void blinkTwice(uint8_t slot, MatrixPixel color);
	void blinkAllTwice(MatrixPixel color);
	void blinkContinuous(uint8_t slot, MatrixPixel color);

	enum Anim { TrackEdit, SampleEdit };
	void playAnim(Anim anim);
	void stopAnim();

	void loop(uint micros) final;
	void loopAnims();

private:
	Matrix* matrix = nullptr;

	enum {
		Single, Anim
	} state = Single;

	enum {
		Solid, Once, Twice, Continuous
	} slotStates[5] = { Solid };

	MatrixPixel slotColors[5] = { MatrixPixel::Off };

	MatrixPixel blinkColors[5] = { MatrixPixel::Off };
	uint32_t blinkStartTimes[5] = { 0 };
	bool blinkStates[5] = { false };
	const uint32_t blinkDuration = 100; //[ms]
	const uint32_t blinkContinuousDuration = 500; //[ms]

	std::array<MatrixAnimGIF*, 2> anims;
};

extern RGBController RGBTrack;
extern RGBController RGBSlot;

#endif //SYNTHIA_FIRMWARE_RGBCONTROLLER_H
