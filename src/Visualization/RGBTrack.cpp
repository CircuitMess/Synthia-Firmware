#include "RGBTrack.h"
#include <Synthia.h>
#include <Devices/Matrix/MatrixAnimGIF.h>

RGBTrackImpl RGBTrack;

RGBTrackImpl::RGBTrackImpl() : TrackLEDController(TrackRGB){

}

void RGBTrackImpl::drawPixel(uint8_t slot, const MatrixPixel& color){
	matrix.drawPixel(0, slot, color);
	matrix.drawPixel(1, slot, color);
}

void RGBTrackImpl::playSpecificAnim(){
	//TODO - add animation path from SPIFFS
	matrix.startAnimation(new MatrixAnimGIF(&matrix, File()));

}
