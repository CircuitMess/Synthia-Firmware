#ifndef CHATTER_FIRMWARE_JIGHWTEST_H
#define CHATTER_FIRMWARE_JIGHWTEST_H

#include <Display/Display.h>
#include <Util/Vector.h>
#include <Input/Input.h>
#include <FS.h>
#include <Util/Task.h>
#include <Devices/Matrix/MatrixPixel.h>

struct Test {
	bool (*test)();
	const char* name;
	void (*onFail)();
};

class JigHWTest
{
public:
	JigHWTest();
	void start();

private:
	static JigHWTest* test;
	Vector<Test> tests;
	const char* currentTest;

	void log(const char* property, const char* value);
	void log(const char* property, float value);
	void log(const char* property, double value);
	void log(const char* property, bool value);
	void log(const char *property, uint32_t value);
	void log(const char *property, const String& value);

	static bool PSRAMTest();
	static bool IS31Test();
	static bool AWTest();
	static bool XL9555Test();
	static bool SPIFFSTest();
	static bool MicTest();
	static bool hwRevision();

	static uint32_t calcChecksum(fs::File& file);

	static const uint32_t PSRAMSize = 4194252; //starting size reported by ESP.getFreePsram() after initialization
	static constexpr uint8_t CurrentVersion = 1;

	void postTestFail();
	void postTestPass();

	static const uint16_t MinMaxDiffThreshold = 30;
	static const int16_t GunkedThreshold = -3000;

};

#endif //CHATTER_FIRMWARE_JIGHWTEST_H
