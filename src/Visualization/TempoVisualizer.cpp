#include "TempoVisualizer.h"
#include <Synthia.h>
#include "TempoAnim.h"

TempoAnim* TempoVisualizer::anim = nullptr;
char text[4];

void TempoVisualizer::visualize(){
	uint8_t value = getProp();
	Matrix& track = Synthia.TrackMatrix;
	Matrix& cursor = Synthia.CursorMatrix;

	if(anim == nullptr){
		anim = new TempoAnim(&cursor);
	}
	if(dynamic_cast<MatrixAnim*>(anim) != cursor.getAnimation()){
		cursor.startAnimation(anim);
	}

	//TODO - add MetronomeAnim on TrackMatrix

	anim->setTempo(value);
	track.clear();
	track.setFont(Matrix::SMALL);
	sprintf(text, "%3d", value);
	track.drawString(5, 5, text);
	track.push();
}
