#include "SampleVisualizer.h"
#include <Synthia.h>

void SampleVisualizer::visualize(){
	auto &matrix = Synthia.TrackMatrix;
	switch(getProp()){
		//TODO - lookup unordered map of animation enums and MatrixAnim pointers
	}
}

void SampleVisualizer::onStop(){
	Synthia.TrackMatrix.stopAnimations();
}

void SampleVisualizer::onStart(){
	//TODO - create all animations on heap
}
