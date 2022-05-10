#ifndef SYNTHIA_FIRMWARE_LEDSTRIP_H
#define SYNTHIA_FIRMWARE_LEDSTRIP_H

#include <cstdint>

class LEDStripImpl {
public:
	/**
	 * Sets the fill for the left slider LED strip, bottom to top
	 * @param value Amount to be filled from bottom to top (0-255)
	 */
	void setLeft(uint8_t value);

	/**
	 * Sets the fill for the right slider LED strip, bottom to top
	 * @param value Amount to be filled from bottom to top (0-255)
	 */
	void setRight(uint8_t value);

	/**
	 * Turns on a single LED on the left slider LED strip
	 * @param value Value to be represented on the strip
	 */
	void setLeftSingle(uint8_t value);

	/**
	 * Turns on a single LED on the right slider LED strip
	 * @param value Value to be represented on the strip
	 */
	void setRightSingle(uint8_t value);

	/**
	 * Sets the fill for the left slider LED strip, starting from the middle
	 * Negative values fill from the center downwards, positive values fill from the center upwards
	 * @param value Amount to be filled from the center (-127 to 128)
	 */
	void setLeftFromCenter(int8_t value);

	/**
	 * Sets the fill for the right slider LED strip, starting from the middle
	 * Negative values fill from the center downwards, positive values fill from the center upwards
	 * @param value Amount to be filled from the center (-127 to 128)
	 */
	void setRightFromCenter(int8_t value);

	/**
	 * Sets the fill for the 6th (cursor) row of LEDs, left to right
	 * @param value Amount to be filled from left to right (0-255)
	 */
	void setMidFill(uint8_t value);

	/**
	* Sets the fill for the 6th (cursor) row of LEDs, starting from the middle
	* Negative values fill from the center to the left, positive values fill from the center to the right
	* @param value Amount to be filled from the center (-127 to 128)
	*/
	void setMidFillFromCenter(int8_t value);

	/**
	 * Turns on a single LED on the 6th (cursor) row of LEDs
	 * @param index Index of the LED to turn on, starting from the left (0-15)
	 */
	void setMidSingle(uint8_t index);

	/**
	 * Turns on all LEDs on one half of the 6th (cursor) row of LEDs.
	 * Value 0 lights up the left half, 1 lights up the right half, other values are ignored
	 * @param value Picks which half to light up (0 - left, 1 - right, other values ignored)
	 */
	void setMidSelection(uint8_t value);

private:
	void setStrip(uint8_t value, uint8_t x);
	void setStripFromCenter(int8_t value, uint8_t x);
	void setStripSingle(uint8_t value, uint8_t x);

};

extern LEDStripImpl LEDStrip;

#endif //SYNTHIA_FIRMWARE_LEDSTRIP_H
