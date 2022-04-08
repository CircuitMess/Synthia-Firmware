#include "SaveVisualizer.h"
#include <Synthia.h>
#include "LEDStrip.h"
#include <SPIFFS.h>

SaveVisualizer::SaveVisualizer() : slotAnim(SPIFFS.open("/test.gif")){}

void SaveVisualizer::visualize(){
	auto data = getProp();
	Matrix& matrix = Synthia.TrackMatrix;

	switch(data.state){
		case SlotSelect:
			for(int x = 13; x < matrix.getWidth(); x++){
				for(int y = 0; y < matrix.getHeight(); y++){
					matrix.drawPixel(x, y, {0, 0, 0, 0});
				}
			}
			matrix.setFont(Matrix::SMALL);
			matrix.drawString(13, 5, String(data.selection));
			matrix.push();

			if(!slotAnim.isStarted()){
				slotAnim.start();
			}
			break;

		case ActionSelect:{
			matrix.stopAnimations();
			matrix.clear();

			//TODO - add save bitmap on left, load bitmap on right, selection is drawn inverted
			LEDStrip.setMidSelection(data.selection);
			break;
		}

		case Confirmation:
			matrix.stopAnimations();
			matrix.clear();

			//TODO - add NO bitmap on left, YES bitmap on right, selection is drawn inverted
			LEDStrip.setMidSelection(data.selection);
			break;
	}
}
