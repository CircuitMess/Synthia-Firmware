#ifndef SYNTHIA_FIRMWARE_RGBDEMOANIM_H
#define SYNTHIA_FIRMWARE_RGBDEMOANIM_H


#include <Devices/Matrix/MatrixAnim.h>
#include <Loop/LoopListener.h>

class RGBDemoAnim : public MatrixAnim, public LoopListener {
public:
	RGBDemoAnim(Matrix& matrix);

	void reset() override;
	void push() override;
	void loop(uint micros) override;

private:
	void onStart() override;
	void onStop() override;

	uint32_t lastTime = 0;
	const uint32_t UpdatePeriod = 500; // [ms]

	static constexpr uint8_t Count = 5; // Track length. Currently for all use-cases of this Anim, it's 5

};


#endif //SYNTHIA_FIRMWARE_RGBDEMOANIM_H
