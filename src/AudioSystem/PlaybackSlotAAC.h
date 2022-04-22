#ifndef SYNTHIA_FIRMWARE_PLAYBACKSLOTAAC_H
#define SYNTHIA_FIRMWARE_PLAYBACKSLOTAAC_H

#include "SampleSlot.h"
#include <FS/RamFile.h>
#include <Audio/SourceAAC.h>
#include <Data/FileDataSource.h>

class PlaybackSlotAAC : public SampleSlot {
public:
	PlaybackSlotAAC(File file);
	~PlaybackSlotAAC() override;

	Generator& getGenerator() override;
	SourceAAC& getSource();

	void seek(size_t pos, SeekMode mode = SeekSet) override;

private:
	FileDataSource ds;
	SourceAAC source;
};


#endif //SYNTHIA_FIRMWARE_PLAYBACKSLOTAAC_H
