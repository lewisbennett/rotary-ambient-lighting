/*
 * @brief Defines the pin used for checking door status.
 */
#define DOOR_PIN 4

/*
 * @brief Defines the pin used for checking headlight status.
 */
#define HEADLIGHT_PIN 5

/*
 * @brief Defines the maximum possible daytime brightness.
 */
#define MAX_BRIGHTNESS_DAY UINT8_MAX

/*
 * @bried Defines the maximum possible nighttime brightness.
 */
#define MAX_BRIGHTNESS_NIGHT 128

/*
 * @brief Defines the minimum possible brightness.
 */
#define MIN_BRIGHTNESS 10

/*
 * @brief Defines the increment during "fast" velocities.
 */
#define ROTARY_ENCODER_INCREMENT_FAST 15

/*
 * @brief Defines the increment during "slow" velocities.
 */
#define ROTARY_ENCODER_INCREMENT_SLOW 1

/*
 * @brief Defines the millisecond threshold to determine a "fast" velocity.
 */
#define ROTARY_ENCODER_MILLIS_FAST 20

/*
 * @brief Defines the millisecond threshold to determine a "slow" velocity.
 */
#define ROTARY_ENCODER_MILLIS_SLOW 200

/*
 * @brief Defines how long the rotary encoders switch (SW) needs to be held for to be considered a "long press", in milliseconds.
 */
#define ROTARY_ENCODER_SW_LONG_PRESS_MILLIS 3000