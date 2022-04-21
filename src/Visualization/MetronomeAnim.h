#ifndef SYNTHIA_FIRMWARE_METRONOMEANIM_H
#define SYNTHIA_FIRMWARE_METRONOMEANIM_H

#include <Devices/Matrix/MatrixAnim.h>
#include <Devices/Matrix/Matrix.h>
#include <Loop/LoopListener.h>
#include <JPEGDecoder.h>
class MetronomeAnim : public MatrixAnim, public LoopListener  {
public:
	MetronomeAnim(Matrix* matrix);

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
	std::pair<uint8_t, uint8_t> startPoint;

	uint8_t tempo = 0;
	float stepTime = 0;

	uint32_t step = 0;
	uint32_t startTime;

	void drawLine();
};


#endif //SYNTHIA_FIRMWARE_METRONOMEANIM_H
