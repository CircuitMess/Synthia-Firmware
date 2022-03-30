#include "SaveVisualizer.h"
#include <Synthia.h>
#include "LEDStrip.h"

void SaveVisualizer::visualize(){
	auto data = getProp();
	Matrix& matrix = Synthia.TrackMatrix;

	switch(data.state){
		case SaveVisData::SlotSelect:
			//TODO - add save/load bitmap drawing on left side of trackMatrix
			matrix.setFont(Matrix::SMALL);
			matrix.drawString(9, 0, String(data.selection));
			break;
		case SaveVisData::ActionSelect:
			//TODO - add save bitmap on left, load bitmap on right, selection is drawn inverted
			LEDStrip.setMidSelection(data.selection);
			break;
		case SaveVisData::Confirmation:
			//TODO - add NO bitmap on left, YES bitmap on right, selection is drawn inverted
			LEDStrip.setMidSelection(data.selection);
			break;
	}
}
