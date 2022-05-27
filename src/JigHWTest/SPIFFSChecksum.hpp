#ifndef CHATTER_FIRMWARE_SPIFFSCHECKSUM_H
#define CHATTER_FIRMWARE_SPIFFSCHECKSUM_H

#include <Arduino.h>

struct {
	String name;
	uint32_t sum;
} static const SPIFFSChecksums[] PROGMEM = {
		{ "/talk.gif",                     13119 },
		{ "/GIF/Save/yes.gif",             10824 },
		{ "/GIF/Save/load.gif",            16916 },
		{ "/GIF/Save/save.gif",            17119 },
		{ "/GIF/Save/diskette.gif",        2829 },
		{ "/GIF/Save/loadSave.gif",        38797 },
		{ "/GIF/Save/no.gif",              10111 },
		{ "/GIF/TrackEdit.gif",            82382 },
		{ "/GIF/Loading.gif",              98998 },
		{ "/GIF/Pitch.gif",                25562 },
		{ "/GIF/Swipe.gif",                98928 },
		{ "/GIF/TrackEditState.gif",       22735 },
		{ "/GIF/Effects/Reverb.gif",       68444 },
		{ "/GIF/Effects/BitCrush.gif",     50691 },
		{ "/GIF/Effects/LowPass.gif",      53345 },
		{ "/GIF/Effects/HighPass.gif",     52307 },
		{ "/GIF/SampleEdit.gif",           161479 },
		{ "/GIF/Intro/Track.gif",          219178 },
		{ "/GIF/Intro/TrackRGB.gif",       74508 },
		{ "/GIF/Intro/SlotRGB.gif",        13556 },
		{ "/GIF/Intro/Sliders.gif",        54582 },
		{ "/GIF/Intro/Cursor.gif",         77898 },
		{ "/GIF/Samples/Snare.gif",        29664 },
		{ "/GIF/Samples/RecOut.gif",       58068 },
		{ "/GIF/Samples/RecWait.gif",      5732 },
		{ "/GIF/Samples/Clap.gif",         46750 },
		{ "/GIF/Samples/RecIn.gif",        62146 },
		{ "/GIF/Samples/Kick.gif",         49153 },
		{ "/GIF/Samples/HatClos.gif",      25495 },
		{ "/GIF/Samples/HatOpen.gif",      23805 },
		{ "/GIF/SampleEditState.gif",      28345 },
		{ "/GIF/Volume.gif",               9981 },
		{ "/HWTestSamples/sliders.aac",    10277094 },
		{ "/HWTestSamples/brightness.aac", 10590362 },
		{ "/HWTestSamples/buttons.aac",    8882101 },
		{ "/HWTestSamples/intro.aac",      6637801 },
		{ "/HWTestSamples/knobs.aac",      15628258 },
		{ "/HWTestSamples/outro.aac",      10250269 },
		{ "/HWTestSamples/intro2.aac",     5574644 },
		{ "/Synthia.aac",                  3947605 },
		{ "/Samples/kick.wav",             3163645 },
		{ "/Samples/snare.wav",            6182525 },
		{ "/Samples/closedhihat.wav",      4703107 },
		{ "/Samples/openhihat.wav",        7094721 },
		{ "/Samples/clap.wav",             8727199 }
};

#endif //CHATTER_FIRMWARE_SPIFFSCHECKSUM_H
