#pragma region Includes

// General sketch configuration.
#include "Configuration.h"

// The sketch's EEPROM configuration.
#include "EEPROM_Configuration.h"

// Arduino EEPROM library.
#include <EEPROM.h>

// LED control library.
#include <FastLED.h>

// The sketch's LED configuration.
#include "LED_Configuration.h"

// Rotary encoder definitions.
#include "RotaryEncoder.h"

#pragma endregion

// EEPROM layout version should not be zero.
#if EEPROM_LAYOUT_VERSION == 0

	#error EEPROM_LAYOUT_VERSION should not be zero.

#endif

#if !defined(LED_ENABLE_ACTIVE) && !defined(LED_ENABLE_PASSIVE)

	#error Either LED_ENABLE_ACTIVE or LED_ENABLE_PASSIVE must be defined.

#endif

enum ADJUSTING_MODE { HUE = 0x01, SATURATION = 0x02, BRIGHTNESS = 0x04 };

#pragma region Variables

// Determines the value to adjust from detected inputs.
uint8_t led_AdjustingMode = HUE;

// Tracks the configured brightness, hue and saturation.
uint8_t led_BrightnessDay, led_BrightnessNight, led_Hue, led_Saturation;

CRGB led_RGB;

// LED control variables.
#ifdef LED_ENABLE_ACTIVE

CLEDController* ledController_Active;
CRGB leds_Active[LED_COUNT_ACTIVE];

#endif

#ifdef LED_ENABLE_PASSIVE

CLEDController* ledController_Passive;
CRGB leds_Passive[LED_COUNT_PASSIVE];

#endif

// Used to calculate the velocity of the rotary encoder for calculating acceleration.
unsigned long rotaryEncoder_LastMillis;

// Used to track the state of the rotary encoder's switch so we can detect new values.
int8_t rotaryEncoder_LastSwitch;

// Used to track when in the future a long press will have been achieved.
unsigned long rotaryEncoder_SwitchLongPressMillis;

#pragma endregion

#pragma region Core Functions

/*
 * @brief Ran once at startup.
 */
void setup() {

	// Initialize the rotary encoder.
	rotaryEncoder_Init();

#ifdef LED_ENABLE_ACTIVE

	ledController_Active = &FastLED.addLeds<LED_TYPE_ACTIVE, LED_PIN_ACTIVE, LED_ORDER_ACTIVE>(leds_Active, LED_COUNT_ACTIVE);

#endif

#ifdef LED_ENABLE_PASSIVE

	ledController_Passive = &FastLED.addLeds<LED_TYPE_PASSIVE, LED_PIN_PASSIVE, LED_ORDER_PASSIVE>(leds_Passive, LED_COUNT_PASSIVE);

#endif
}

/*
 * @brief Runs continuously.
 */
void loop() {

	unsigned long now = millis();

	// Handle press of rotary encoder's switch.
	int8_t rotaryEncoder_Switch = rotaryEncoder_ReadSwitch();

	if (rotaryEncoder_Switch == 1) {

		// Handle single press of the switch.
		if (rotaryEncoder_Switch != rotaryEncoder_LastSwitch) {

			led_AdjustingMode = (led_AdjustingMode & HUE) == 1 ? SATURATION : HUE;

			// Set the long press millis.
			rotaryEncoder_SwitchLongPressMillis = now + ROTARY_ENCODER_SW_LONG_PRESS_MILLIS;
		}
		// At this point, the switch is held. Check if we've exceeded the required time delay before handling.
		else if (now > rotaryEncoder_SwitchLongPressMillis) {

			// Long pressing resets the displayed color to white.
			// This is the only way to access this color, so enable adjusting brightness
			// mode to avoid accidentally changing the hue immediately afterwards.
			led_AdjustingMode = BRIGHTNESS;

			// Set to maximum possible value to avoid running this when unwanted.
			rotaryEncoder_SwitchLongPressMillis = UINT32_MAX;

			return;
		}
	}

	rotaryEncoder_LastSwitch = rotaryEncoder_Switch;

	// Handle rotation changes of the rotary encoder.
	int8_t rotaryEncoder_Value;

	if (rotaryEncoder_Value = rotaryEncoder_ReadNewRotation()) {

		unsigned long millisDelta = now - rotaryEncoder_LastMillis;

		int8_t increment;

		if (millisDelta > ROTARY_ENCODER_MILLIS_SLOW)
			increment = ROTARY_ENCODER_INCREMENT_SLOW;

		else if (millisDelta < ROTARY_ENCODER_INCREMENT_FAST)
			increment = ROTARY_ENCODER_INCREMENT_FAST;

		else {

			// Calculate the correct increment based on the distance between the
			// 'fast' and 'slow' velocities. Uses */ 100 to avoid floating point math.
			increment = ROTARY_ENCODER_INCREMENT_FAST - ((ROTARY_ENCODER_INCREMENT_FAST - ROTARY_ENCODER_INCREMENT_SLOW)
				* ((millisDelta / (ROTARY_ENCODER_MILLIS_SLOW - ROTARY_ENCODER_MILLIS_FAST) * 100))) / 100;
		}

		rotaryEncoder_LastMillis = now;

		increment *= rotaryEncoder_Value;

		// Adjust either the hue or brightness depending on the current mode.
		switch (led_AdjustingMode) {

			case BRIGHTNESS:

				// TODO
				// Check if the headlights are on. A different value should be altered depending on the state.
				if (true)
					incrementWithoutOverflow(&led_BrightnessDay, increment);

				else
					incrementWithoutOverflow(&led_BrightnessNight, increment);

				break;

			case HUE:
				led_Hue += increment;
				break;

			case SATURATION:
				incrementWithoutOverflow(&led_Saturation, increment);
				break;
		}

		hsv2rgb_rainbow(CHSV(led_Hue, led_Saturation, UINT8_MAX), led_RGB);

		led_ShowColor();
	}
}

#pragma endregion

void incrementWithoutOverflow(uint8_t* in, int8_t increment) {

	if (increment > 0 && uint8_t(in + increment) < *in)
		*in = UINT8_MAX;

	else if (increment < 0 && uint8_t(in + increment) > *in)
		*in = 0;

	else
		*in += increment;
}

void led_ShowColor() {

#ifdef LED_ENABLE_ACTIVE

	// TODO
	// Check whether door is open, as we can't change the color if it is.
	// Also, check whether to use day or night brightness setting.
	if (true)
		ledController_Active->showColor(led_RGB, led_BrightnessDay);

#endif

#ifdef LED_ENABLE_PASSIVE

	ledController_Passive->showColor(led_RGB, led_BrightnessDay);

#endif
}