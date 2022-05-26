#ifndef SYNTHIA_FIRMWARE_TIMEOUT_H
#define SYNTHIA_FIRMWARE_TIMEOUT_H

#include <Arduino.h>
#include <Loop/LoopListener.h>
#include <functional>

class Timeout : public LoopListener {
public:
	Timeout(uint32_t time, std::function<void()>  callback);
	void loop(uint micros) override;
	virtual ~Timeout();

private:
	uint32_t start;
	uint32_t time;
	bool selfDelete = true;
	std::function<void()> callback;
};


#endif //SYNTHIA_FIRMWARE_TIMEOUT_H
