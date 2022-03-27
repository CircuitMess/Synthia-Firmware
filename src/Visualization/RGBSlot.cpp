#include "RGBSlot.h"
#include <Synthia.h>
#include <Devices/Matrix/MatrixAnimGIF.h>

RGBSlotImpl RGBSlot;

RGBSlotImpl::RGBSlotImpl() : TrackLEDController(SlotRGB){

}

void RGBSlotImpl::drawPixel(uint8_t slot, const MatrixPixel& color){
	matrix.drawPixel(slot, color);
}

void RGBSlotImpl::playSpecificAnim(){
	//TODO - add animation path from SPIFFS
	matrix.startAnimation(new MatrixAnimGIF(&matrix, File()));
}
