#ifndef SYNTHIA_FIRMWARE_PLAYBACKSLOT_H
#define SYNTHIA_FIRMWARE_PLAYBACKSLOT_H

#include "SampleSlot.h"
#include <FS/RamFile.h>
#include <Audio/SourceAAC.h>
#include <Data/FileDataSource.h>

class PlaybackSlot : public SampleSlot {
public:
	PlaybackSlot(File file);
	~PlaybackSlot() override;

	Generator& getGenerator() override;
	SourceAAC& getSource();

	void seek(size_t pos, SeekMode mode = SeekSet) override;

private:
	FileDataSource ds;
	SourceAAC source;
};


#endif //SYNTHIA_FIRMWARE_PLAYBACKSLOT_H
