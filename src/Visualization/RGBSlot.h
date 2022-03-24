#ifndef SYNTHIA_FIRMWARE_RGBSLOT_H
#define SYNTHIA_FIRMWARE_RGBSLOT_H


#include "TrackLEDController.h"

class RGBSlotImpl : public TrackLEDController {
public:
	RGBSlotImpl();

protected:
	void drawPixel(uint8_t slot, const MatrixPixel& color) override;
	void playSpecificAnim() override;
};

extern RGBSlotImpl RGBSlot;

#endif //SYNTHIA_FIRMWARE_RGBSLOT_H
