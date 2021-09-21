/*
 * @brief Defines the pin used for checking door status.
 */
#define DOOR_PIN 4

/*
 * @brief Defines the pin used for checking headlight status.
 */
#define HEADLIGHT_PIN 5

/*
 * @brief Defines the total number of LEDs being controlled in the footwell.
 */
#define LED_COUNT_DOOR 12

/*
 * @brief Defines the LED control order in the footwell.
 */
#define LED_ORDER_DOOR GRB

/*
 * @brief Defines the digital pin (not physical pin) to use for LED output in the footwell.
 */
#define LED_PIN_DOOR 7

/*
 * @brief Defines the type of LED being controlled in the footwell.
 */
#define LED_TYPE_DOOR WS2812

/*
 * @brief Defines the total number of LEDs being controlled in the footwell.
 */
#define LED_COUNT_FOOTWELL 16

/*
 * @brief Defines the LED control order in the footwell.
 */
#define LED_ORDER_FOOTWELL GRB

/*
 * @brief Defines the digital pin (not physical pin) to use for LED output in the footwell.
 */
#define LED_PIN_FOOTWELL 6

/*
 * @brief Defines the type of LED being controlled in the footwell.
 */
#define LED_TYPE_FOOTWELL WS2812

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
#define ROTARY_INCREMENT_FAST 15

/*
 * @brief Defines the increment during "slow" velocities.
 */
#define ROTARY_INCREMENT_SLOW 1

/*
 * @brief Defines the millisecond threshold to determine a "fast" velocity.
 */
#define ROTARY_MILLIS_FAST 20

/*
 * @brief Defines the millisecond threshold to determine a "slow" velocity.
 */
#define ROTARY_MILLIS_SLOW 200

/*
 * @brief Defines how long the rotary encoders switch (SW) needs to be held for to be considered a "long press", in milliseconds.
 */
#define ROTARY_SW_LONG_PRESS_MILLIS 3000