#ifndef SYNTHIA_FIRMWARE_INTRO_H
#define SYNTHIA_FIRMWARE_INTRO_H

#include <Arduino.h>
#include <Devices/Matrix/MatrixAnim.h>
#include "State.h"
#include "../Model/SlotConfig.h"
#include "../AudioSystem/EditSlot.h"
#include "../AudioSystem/Baker.h"
#include <memory>
#include <Devices/Matrix/MatrixAnimGIF.h>

class Intro : public State, public LoopListener {
protected:
	void onStart() override;
	void onStop() override;

public:
	void loop(uint micros) override;

private:
	struct AnimMapping {
		Matrix& matrix;
		const char* filename;
	};
	static const AnimMapping IntroAnims[5];

	std::unique_ptr<Baker> baker;
	std::array<std::unique_ptr<MatrixAnimGIF>, 5> anims;
	std::unique_ptr<MatrixAnimGIF> loadingAnim;

	File bakedFiles[5];

	bool introAnimDone = false;
};


#endif //SYNTHIA_FIRMWARE_INTRO_H
