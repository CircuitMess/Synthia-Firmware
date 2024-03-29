#ifndef CHATTER_FIRMWARE_SPIFFSCHECKSUM_H
#define CHATTER_FIRMWARE_SPIFFSCHECKSUM_H
#include <Arduino.h>

struct {
	String name;
	uint32_t sum;
} static const SPIFFSChecksums[] PROGMEM = {
		{ "/GIF/Effects/BitCrush.gif", 10639},
		{ "/GIF/Effects/HighPass.gif", 15824},
		{ "/GIF/Effects/LowPass.gif", 5733},
		{ "/GIF/Effects/Reverb.gif", 13893},
		{ "/GIF/Intro/Cursor.gif", 77898},
		{ "/GIF/Intro/Sliders.gif", 54582},
		{ "/GIF/Intro/SlotRGB.gif", 13556},
		{ "/GIF/Intro/Track.gif", 46318},
		{ "/GIF/Intro/TrackRGB.gif", 74508},
		{ "/GIF/Loading.gif", 25376},
		{ "/GIF/Pitch.gif", 25562},
		{ "/GIF/SampleEdit.gif", 51783},
		{ "/GIF/SampleEditState.gif", 28345},
		{ "/GIF/Samples/Clap.gif", 34311},
		{ "/GIF/Samples/HatClos.gif", 11641},
		{ "/GIF/Samples/HatOpen.gif", 23805},
		{ "/GIF/Samples/Kick.gif", 25962},
		{ "/GIF/Samples/RecIn.gif", 62146},
		{ "/GIF/Samples/RecOut.gif", 31653},
		{ "/GIF/Samples/RecWait.gif", 5732},
		{ "/GIF/Samples/Snare.gif", 29664},
		{ "/GIF/Save/diskette.gif", 2829},
		{ "/GIF/Save/load.gif", 16916},
		{ "/GIF/Save/loadSave.gif", 38797},
		{ "/GIF/Save/no.gif", 10111},
		{ "/GIF/Save/save.gif", 17119},
		{ "/GIF/Save/yes.gif", 10824},
		{ "/GIF/Swipe.gif", 7424},
		{ "/GIF/TrackEdit.gif", 36824},
		{ "/GIF/TrackEditState.gif", 22735},
		{ "/GIF/Volume.gif", 9981},
		{ "/HWTestSamples/brightness.aac", 10783},
		{ "/HWTestSamples/buttons.aac", 10065},
		{ "/HWTestSamples/intro.aac", 41132},
		{ "/HWTestSamples/intro2.aac", 42454},
		{ "/HWTestSamples/knobs.aac", 49340},
		{ "/HWTestSamples/outro.aac", 38746},
		{ "/HWTestSamples/sliders.aac", 63077},
		{ "/Samples/clap.wav", 6884},
		{ "/Samples/closedhihat.wav", 15693},
		{ "/Samples/kick.wav", 110087},
		{ "/Samples/openhihat.wav", 7972},
		{ "/Samples/snare.wav", 4433},
		{ "/Synthia.aac", 75621},
		{ "/talk.gif", 13119}
};

#endif //CHATTER_FIRMWARE_SPIFFSCHECKSUM_H
