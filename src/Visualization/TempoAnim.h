#ifndef SYNTHIA_FIRMWARE_TEMPOANIM_H
#define SYNTHIA_FIRMWARE_TEMPOANIM_H

#include <Arduino.h>
#include <Devices/Matrix/MatrixAnim.h>
#include <Devices/Matrix/Matrix.h>
#include <Loop/LoopListener.h>

class TempoAnim : public MatrixAnim, public LoopListener {
public:
	TempoAnim(Matrix* matrix);

	void reset() override;

	void setTempo(uint8_t tempo);
	uint8_t getTempo() const;

	void loop(uint micros) override;
	void push() override;

protected:
	void onStart() override;
	void onStop() override;

private:
	uint16_t steps;

	uint8_t tempo = 0;
	float stepTime = 0;

	uint32_t step = 0;
	uint32_t startTime;

};


#endif //SYNTHIA_FIRMWARE_TEMPOANIM_H
