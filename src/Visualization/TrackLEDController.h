#ifndef SYNTHIA_FIRMWARE_TRACKLEDCONTROLLER_H
#define SYNTHIA_FIRMWARE_TRACKLEDCONTROLLER_H

#include <Loop/LoopListener.h>
#include <Devices/Matrix/MatrixPixel.h>

class Matrix;

class TrackLEDController : public LoopListener {
public:
	TrackLEDController(Matrix& matrix);
	void setColor(uint8_t slot, MatrixPixel color);
	void clear();
	void blink(uint8_t slot, MatrixPixel color);
	void blinkAll(MatrixPixel color);
	void blinkTwice(uint8_t slot, MatrixPixel color);
	void blinkAllTwice(MatrixPixel color);
	void blinkContinuous(uint8_t slot, MatrixPixel color);
	void playAnim();

	void loop(uint micros) final;

protected:
	virtual void drawPixel(uint8_t slot, const MatrixPixel& color) = 0;
	virtual void playSpecificAnim() = 0;

	Matrix& matrix;

private:

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
	const uint32_t blinkTime = 350; //[ms]
};


#endif //SYNTHIA_FIRMWARE_TRACKLEDCONTROLLER_H
