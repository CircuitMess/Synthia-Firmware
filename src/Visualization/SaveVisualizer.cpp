#include "SaveVisualizer.h"
#include <Synthia.h>
#include "LEDStrip.h"
#include <Devices/Matrix/MatrixAnimGIF.h>
#include <SPIFFS.h>

void SaveVisualizer::visualize(){
	auto data = getProp();
	Matrix& matrix = Synthia.TrackMatrix;

	switch(data.state){
		case SlotSelect:
			//iterate by x and y to clear pixels from (13, 0) to (16, 5)
			for(int x = 13; x < matrix.getWidth(); x++){
				for(int y = 0; y < matrix.getHeight(); y++){
					matrix.drawPixel(x, y, {0, 0, 0, 0});
				}
			}
			matrix.startAnimation(new MatrixAnimGIF(&matrix, SPIFFS.open("/test.gif")));
			matrix.setFont(Matrix::SMALL);
			matrix.drawString(13, 5, String(data.selection));
			matrix.push();
			break;

		case ActionSelect:{
			matrix.stopAnimation();
			matrix.clear();
			auto anim = new MatrixAnimGIF(&matrix, SPIFFS.open("/test.gif"));
			anim->setX(data.selection ?  9 : 1);
			matrix.startAnimation(anim);
			//TODO - add save bitmap on left, load bitmap on right, selection is drawn inverted
			LEDStrip.setMidSelection(data.selection);
			break;
		}

		case Confirmation:
			matrix.stopAnimation();
			matrix.clear();
			auto anim = new MatrixAnimGIF(&matrix, SPIFFS.open("/test.gif"));
			anim->setX(data.selection ?  9 : 1);
			matrix.startAnimation(anim);
			//TODO - add NO bitmap on left, YES bitmap on right, selection is drawn inverted
			LEDStrip.setMidSelection(data.selection);
			break;
	}
}
