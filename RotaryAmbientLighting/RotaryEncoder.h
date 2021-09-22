#ifndef RotaryEncoder
#define RotaryEncoder

/*
 * @brief Initializes the rotary encoder.
 */
void rotaryEncoder_Init();

/*
 * @brief Reads the new rotational value of the rotary encoder, if available.
 * Returns 1 for valid clockwise movement.
 * Returns -1 for valid anticlockwise movement.
 * Returns 0 for invalid movement.
 */
int8_t rotaryEncoder_ReadNewRotation();

/*
 * @brief Reads the value of the rotary encoder's switch.
 * Returns 1 for valid 'switch pressed'.
 * Returns -1 for valid 'switch not pressed'.
 * Returns 0 for invalid switch state.
 */
int8_t rotaryEncoder_ReadSwitch();

#endif