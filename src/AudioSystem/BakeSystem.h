#ifndef SYNTHIA_FIRMWARE_BAKESYSTEM_H
#define SYNTHIA_FIRMWARE_BAKESYSTEM_H

#include <FS/RamFile.h>
#include "EditSlot.h"

class BakeSystem {
public:
	File bake(EditSlot* slot);
};

extern BakeSystem SampleBaker;

#endif //SYNTHIA_FIRMWARE_BAKESYSTEM_H
