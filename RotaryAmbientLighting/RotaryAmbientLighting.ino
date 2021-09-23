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

#pragma region Variables

// Determines whether alterations to the rotary encoder's rotation affect hue or brightness.
boolean adjustingBrightness;

// Tracks the configured hue.
uint8_t led_Hue;

// LED control variables.
#ifdef LED_ENABLE_ACTIVE

uint8_t led_ActiveConfiguredBrightness, led_ActiveCurrentBrightness;
CLEDController* ledController_Active;
CRGB leds_Active[LED_COUNT_ACTIVE], led_ActiveConfiguredRGB, led_ActiveCurrentRGB;

#endif

#ifdef LED_ENABLE_PASSIVE

uint8_t led_PassiveCurrentBrightness;
CLEDController* ledController_Passive;
CRGB leds_Passive[LED_COUNT_PASSIVE], led_PassiveCurrentRGB;

#endif

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

#ifdef LED_ENABLE_ACTIVE

	ledController_Active = &FastLED.addLeds<LED_TYPE_ACTIVE, LED_PIN_ACTIVE, LED_ORDER_ACTIVE>(leds_Active, LED_COUNT_ACTIVE);

#endif

#ifdef LED_ENABLE_PASSIVE

	ledController_Passive = &FastLED.addLeds<LED_TYPE_PASSIVE, LED_PIN_PASSIVE, LED_ORDER_PASSIVE>(leds_Passive, LED_COUNT_PASSIVE);

#endif
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
			rotaryEncoder_SWLongPressMillis = now + ROTARY_ENCODER_SW_LONG_PRESS_MILLIS;
		}
		// At this point, the switch is held. Check if we've exceeded the required time delay before handling.
		else if (now > rotaryEncoder_SWLongPressMillis) {

			// Long pressing resets the displayed color to white.
			// This is the only way to access this color, so enable adjusting brightness
			// mode to avoid accidentally changing the hue immediately afterwards.
			adjustingBrightness = true;

#ifdef LED_ENABLE_ACTIVE

			led_ActiveConfiguredBrightness = UINT8_MAX;
			led_ActiveConfiguredRGB = CRGB(UINT8_MAX, UINT8_MAX, UINT8_MAX);

#endif

#ifdef LED_ENABLE_PASSIVE

			led_PassiveCurrentBrightness = UINT8_MAX;
			led_PassiveCurrentRGB = CRGB(UINT8_MAX, UINT8_MAX, UINT8_MAX);

#endif

			led_ShowColor();

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
		if (adjustingBrightness) {

#ifdef LED_ENABLE_ACTIVE

			led_ActiveConfiguredBrightness += increment;

#endif

#ifdef LED_ENABLE_PASSIVE

			led_PassiveCurrentBrightness += increment;

#endif
		}
		else {

			led_Hue += (increment * rotaryEncoder_Value);

			CRGB newRgb;

			hsv2rgb_rainbow(CHSV(led_Hue, UINT8_MAX, UINT8_MAX), newRgb);

#ifdef LED_ENABLE_ACTIVE

			led_ActiveConfiguredRGB = newRgb;

#endif

#ifdef LED_ENABLE_PASSIVE

			led_PassiveCurrentRGB = newRgb;

#endif
		}

		led_ShowColor();
	}
}

#pragma endregion

void led_ShowColor() {

#ifdef LED_ENABLE_ACTIVE

	// TODO
	// Check whether door is open, as we can't change the color if it is.
	if (true)
		ledController_Active->showColor(led_ActiveConfiguredRGB, led_ActiveConfiguredBrightness);

#endif

#ifdef LED_ENABLE_PASSIVE

	ledController_Passive->showColor(led_PassiveCurrentRGB, led_PassiveCurrentBrightness);

#endif
}