#ifndef SYNTHIA_FIRMWARE_RGBTRACK_H
#define SYNTHIA_FIRMWARE_RGBTRACK_H

#include "TrackLEDController.h"

class RGBTrackImpl : public TrackLEDController {
public:
	RGBTrackImpl();

protected:
	void drawPixel(uint8_t slot, const MatrixPixel& color) override;
	void playSpecificAnim() override;
};

extern RGBTrackImpl RGBTrack;

#endif //SYNTHIA_FIRMWARE_RGBTRACK_H
