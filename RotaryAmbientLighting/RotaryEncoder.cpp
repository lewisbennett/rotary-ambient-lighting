#pragma region Includes

// Standard Arduino libraries.
#include <Arduino.h>

// Rotary encoder definitions.
#include "RotaryEncoder.h"

// Rotary encoder configuration.
#include "RotaryEncoder_Configuration.h"

#pragma endregion

#pragma region Variables

// A perfect rotation sequence will look like this.
// We'll compare readings from the encoder to this to see whether they're valid or not.
uint8_t rotaryEncoder_Table[] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 };

// Not 100% sure on the actual purpose of these variables.
// They're extremely important, but I don't know how to describe their purpose.
uint8_t prevNextCode = 0;
uint16_t store = 0;

//
unsigned long sw_DebounceMillis;

#pragma endregion

#pragma region Defined Functions

/*
 * @brief Initializes the rotary encoder.
 */
void rotaryEncoder_Init() {

	uint8_t clk_Port = digitalPinToPort(ROTARY_ENCODER_CLK_PIN);
	uint8_t clk_BitMask = digitalPinToBitMask(ROTARY_ENCODER_CLK_PIN);

	// Configure pin mode for CLK pin.
	*portModeRegister(clk_Port) &= ~clk_BitMask;		// Set CLK pin to input.
	*portOutputRegister(clk_Port) |= clk_BitMask;		// Enable internal pull-up resistor on CLK pin.

	uint8_t dt_Port = digitalPinToPort(ROTARY_ENCODER_DT_PIN);
	uint8_t dt_BitMask = digitalPinToBitMask(ROTARY_ENCODER_DT_PIN);

	*portModeRegister(dt_Port) &= ~dt_BitMask;		// Set DT pin to input.
	*portOutputRegister(dt_Port) |= dt_BitMask;		// Enable internal pull-up resistor on DT pin.

	uint8_t sw_Port = digitalPinToPort(ROTARY_ENCODER_SW_PIN);
	uint8_t sw_BitMask = digitalPinToBitMask(ROTARY_ENCODER_SW_PIN);

	*portModeRegister(sw_Port) &= ~sw_BitMask;		// Set SW pin to input.
	*portOutputRegister(sw_Port) |= sw_BitMask;		// Enable internal pull-up resistor on SW pin.
}

/*
 * @brief Reads the new rotational value of the rotary encoder, if available.
 * Returns 1 for valid clockwise movement.
 * Returns -1 for valid anticlockwise movement.
 * Returns 0 for invalid movement.
 */
int8_t rotaryEncoder_ReadNewRotation() {

	prevNextCode <<= 2;

	// Read the value of the DT pin.
	if (*portInputRegister(digitalPinToPort(ROTARY_ENCODER_CLK_PIN)) & digitalPinToBitMask(ROTARY_ENCODER_DT_PIN))
		prevNextCode |= 0x02;

	// Read the value of the CLK pin.
	if (*portInputRegister(digitalPinToPort(ROTARY_ENCODER_CLK_PIN)) & digitalPinToBitMask(ROTARY_ENCODER_CLK_PIN))
		prevNextCode |= 0x01;

	prevNextCode &= 0x0f;

	// If valid, store as 16 bit data.
	if (rotaryEncoder_Table[prevNextCode]) {

		store <<= 4;
		store |= prevNextCode;

		// Check if anticlockwise movement.
		if ((store & 0xff) == 0x2b)
			return -1;

		// Check if clockwise movement.
		if ((store & 0xff) == 0x17)
			return 1;
	}

	// Reading was invalid, return 0.
	return 0;
}

/*
 * @brief Reads the value of the rotary encoder's switch.
 */
boolean rotaryEncoder_ReadSwitch() {

	// Read the value of the SW pin. It has to be low to be registered as being pressed.
	uint8_t sw_State = *portInputRegister(digitalPinToPort(ROTARY_ENCODER_SW_PIN)) & digitalPinToBitMask(ROTARY_ENCODER_SW_PIN);

	// Only return 'true' if the state is low and the debounce window has passed.
	if (sw_State == LOW) {

		unsigned long now = millis();

		if (now > sw_DebounceMillis) {

			// Set to maximum possible value to avoid running this when unwanted.
			sw_DebounceMillis = UINT32_MAX;

			return true;
		}

		// Set the debounce millis.
		sw_DebounceMillis = now + ROTARY_ENCODER_SW_DEBOUNCE_MILLIS;
	}

	return false;
}

#pragma endregion
