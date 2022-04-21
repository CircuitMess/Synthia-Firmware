#ifndef SYNTHIA_FIRMWARE_EFFECTVISUALIZER_H
#define SYNTHIA_FIRMWARE_EFFECTVISUALIZER_H

#include "Visualizer.h"
#include "../Model/EffectData.hpp"
#include <Devices/Matrix/MatrixAnimGIF.h>

class EffectVisualizer : public Visualizer<EffectData>{
protected:
public:
	EffectVisualizer();

protected:
	void visualize() override;

	void onStart() override;
	void onStop() override;

private:
	static const char* Anims[(size_t) EffectData::Type::COUNT];
	std::vector<std::unique_ptr<MatrixAnimGIF>> anims;

};


#endif //SYNTHIA_FIRMWARE_EFFECTVISUALIZER_H
