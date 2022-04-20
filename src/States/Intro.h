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
#include <Audio/OutputI2S.h>
#include <Audio/SourceAAC.h>

class Intro : public State, public LoopListener {
public:
	Intro();
	void loop(uint micros) override;

protected:
	void onStart() override;
	void onStop() override;

private:
	struct AnimMapping {
		Matrix& matrix;
		const char* filename;
		bool loop;
	};
	static const AnimMapping IntroAnims[5];

	std::unique_ptr<Baker> baker;
	std::array<std::unique_ptr<MatrixAnimGIF>, 5> anims;
	std::unique_ptr<MatrixAnimGIF> loadingAnim;

	bool introAnimDone = false;

	File audioFile;
	FileDataSource fileSource;
	SourceAAC source;
	OutputI2S output;

	void launch();
};


#endif //SYNTHIA_FIRMWARE_INTRO_H
