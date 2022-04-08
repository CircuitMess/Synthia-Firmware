#include "TrackVisualizer.h"
#include <Synthia.h>
#include "LEDStrip.h"

void TrackVisualizer::visualize(){
	auto data = getProp();
	for(int y = 0; y < Synthia.TrackMatrix.getHeight(); y++){
		for(int x = 0; x < Synthia.TrackMatrix.getWidth(); x++){
			uint8_t val = min(255, data.timeline.get(x, y) * 255 + (100 * (data.cursor == x)));
			Synthia.TrackMatrix.drawPixel(x, y, { val, val, val, val });
		}
	}
	Synthia.TrackMatrix.push();
	LEDStrip.setMidSingle(data.cursor);
}
