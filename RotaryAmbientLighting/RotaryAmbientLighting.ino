#pragma region Includes

// General sketch configuration.
#include "Configuration.h"

// The sketch's EEPROM configuration.
#include "EEPROM_Configuration.h"

// Arduino EEPROM library.
#include <EEPROM.h>

// LED control library.
#include <FastLED.h>

// Rotary encoder definitions.
#include "RotaryEncoder.h"

#pragma endregion

// EEPROM layout version should not be zero.
#if EEPROM_LAYOUT_VERSION == 0

	#error EEPROM_LAYOUT_VERSION should not be zero.

#endif

#pragma region Variables

// Determines whether alterations to the rotary encoder's rotation affect hue or brightness.
boolean adjustingBrightness;

// Values that can be modified for LED display.
uint8_t led_Brightness, led_Hue;
CRGB led_RGB;

// LED control variables.
CLEDController* ledController_Door, * ledController_Footwell;
CRGB leds_Door[LED_COUNT_DOOR], leds_Footwell[LED_COUNT_FOOTWELL];

// Used to calculate the velocity of the rotary encoder for calculating acceleration.
unsigned long rotaryEncoder_LastMillis;

// Used to track the state of the rotary encoder's switch so we can detect new values.
boolean rotaryEncoder_LastSwitch;

#pragma endregion

#pragma region Core Functions

/*
 * @brief Ran once at startup.
 */
void setup() {

	// Initialize the rotary encoder.
	rotaryEncoder_Init();

	ledController_Door = &FastLED.addLeds<LED_TYPE_DOOR, LED_PIN_DOOR, LED_ORDER_DOOR>(leds_Door, LED_COUNT_DOOR);
	ledController_Footwell = &FastLED.addLeds<LED_TYPE_FOOTWELL, LED_PIN_FOOTWELL, LED_ORDER_FOOTWELL>(leds_Footwell, LED_COUNT_FOOTWELL);
}

int8_t rotaryEncoder_LastSW;

unsigned long rotaryEncoder_SWLongPressMillis;

/*
 * @brief Runs continuously.
 */
void loop() {

	unsigned long now = millis();

	// Handle press of rotary encoder's switch.
	int8_t rotaryEncoder_SW = rotaryEncoder_ReadSwitch();

	if (rotaryEncoder_SW == 1) {

		// Handle single press of the switch.
		if (rotaryEncoder_SW != rotaryEncoder_LastSW) {

			adjustingBrightness = !adjustingBrightness;

			// Set the long press millis.
			rotaryEncoder_SWLongPressMillis = now + ROTARY_SW_LONG_PRESS_MILLIS;
		}
		// At this point, the switch is held. Check if we've exceeded the required time delay before handling.
		else if (now > rotaryEncoder_SWLongPressMillis) {

			// Long pressing resets the displayed color to white.
			// This is the only way to access this color, so enable adjusting brightness
			// mode to avoid accidentally changing the hue immediately afterwards.
			adjustingBrightness = true;

			led_Brightness = UINT8_MAX;
			led_RGB = CRGB(UINT8_MAX, UINT8_MAX, UINT8_MAX);

			ledController_Door->showColor(led_RGB, led_Brightness);

			// Set to maximum possible value to avoid running this when unwanted.
			rotaryEncoder_SWLongPressMillis = UINT32_MAX;

			return;
		}
	}

	rotaryEncoder_LastSW = rotaryEncoder_SW;

	// Handle rotation changes of the rotary encoder.
	int8_t rotaryEncoder_Value;

	if (rotaryEncoder_Value = rotaryEncoder_ReadNewRotation()) {

		unsigned long millisDelta = now - rotaryEncoder_LastMillis;

		int8_t increment;

		if (millisDelta > ROTARY_MILLIS_SLOW)
			increment = ROTARY_INCREMENT_SLOW;

		else if (millisDelta < ROTARY_INCREMENT_FAST)
			increment = ROTARY_INCREMENT_FAST;

		else {

			// Calculate the correct increment based on the distance between the
			// 'fast' and 'slow' velocities. Uses */ 100 to avoid floating point math.
			increment = ROTARY_INCREMENT_FAST - ((ROTARY_INCREMENT_FAST - ROTARY_INCREMENT_SLOW)
				* ((millisDelta / (ROTARY_MILLIS_SLOW - ROTARY_MILLIS_FAST) * 100))) / 100;
		}

		rotaryEncoder_LastMillis = now;

		// Adjust either the hue or brightness depending on the current mode.
		if (adjustingBrightness)
			led_Brightness += (increment * rotaryEncoder_Value);

		else {

			led_Hue += (increment * rotaryEncoder_Value);

			hsv2rgb_rainbow(CHSV(led_Hue, UINT8_MAX, UINT8_MAX), led_RGB);
		}

		ledController_Door->showColor(led_RGB, led_Brightness);
	}
}

#pragma endregion
