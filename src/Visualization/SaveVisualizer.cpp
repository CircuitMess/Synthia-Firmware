#include "SaveVisualizer.h"
#include <Synthia.h>
#include "LEDStrip.h"
#include <SPIFFS.h>

SaveVisualizer::SaveVisualizer() : slotAnim(SPIFFS.open("/GIF/Save/loadSave.gif"), &Synthia.TrackMatrix),
								   no(SPIFFS.open("/GIF/Save/no.gif"), &Synthia.TrackMatrix),
								   yes(SPIFFS.open("/GIF/Save/yes.gif"), &Synthia.TrackMatrix),
								   load(SPIFFS.open("/GIF/Save/load.gif"), &Synthia.TrackMatrix),
								   save(SPIFFS.open("/GIF/Save/save.gif"), &Synthia.TrackMatrix),
								   diskette(SPIFFS.open("/GIF/Save/diskette.gif"), &Synthia.TrackMatrix){
	load.setX(9);
	save.setX(-1);
	yes.setX(8);
}

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
			matrix.drawString(13, 5, String(data.selection + 1));
			matrix.push();

			if(!slotAnim.isStarted()){
				slotAnim.start();
			}
			break;

		case ActionSelect:{
			if(slotAnim.isStarted()) slotAnim.stop();

			if(data.selection && !load.isStarted()){
				save.reset();
				save.push();
				save.stop();
				load.start();
			}else if(!data.selection && !save.isStarted()){
				load.reset();
				load.push();
				load.stop();
				save.start();
			}

			LEDStrip.setMidSelection(data.selection);
			break;
		}

		case Confirmation:
			if(save.isStarted()) save.stop();
			if(load.isStarted()) load.stop();

			if(data.selection){
				no.reset();
				no.push();
				no.stop();
				yes.start();
			}else{
				yes.reset();
				yes.push();
				yes.stop();
				no.start();
			}
			LEDStrip.setMidSelection(data.selection);
			break;

		case Wait:
			matrix.stopAnimations();
			matrix.clear();
			if(!diskette.isStarted()){
				diskette.start();
			}
			if(data.selection && !load.isStarted()){
				load.setX(9);
				load.start();
			}else if(!data.selection && !save.isStarted()){
				save.setX(8);
				save.start();
			}
			break;
	}
}

void SaveVisualizer::onStop(){
	Synthia.TrackMatrix.stopAnimations();
	Synthia.TrackMatrix.clear();

	Synthia.CursorMatrix.clear();
}

void SaveVisualizer::onStart(){
	Synthia.TrackMatrix.clear();
	Synthia.CursorMatrix.clear();
	Synthia.CursorMatrix.push();
}
