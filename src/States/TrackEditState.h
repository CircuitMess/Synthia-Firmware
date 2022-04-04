#ifndef SYNTHIA_FIRMWARE_TRACKEDITSTATE_H
#define SYNTHIA_FIRMWARE_TRACKEDITSTATE_H

#include "State.h"
#include "../Model/Track.h"

class TrackEditState : public State {
public:
	void setTrack(Track track);
	Track& getTrack();
protected:
	void onStart() override;
	void onStop() override;
	Track track;
};


#endif //SYNTHIA_FIRMWARE_TRACKEDITSTATE_H
