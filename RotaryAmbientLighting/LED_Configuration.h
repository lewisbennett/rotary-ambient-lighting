/*
 * @brief Defines the digital pin to use for 'active LEDs'. Requires ENABLE_ACTIVE_LEDS to be defined.
 */
#define LED_COUNT_ACTIVE 12

/*
 * @brief Defines the digital pin to use for 'passive LEDs'.
 */
#define LED_COUNT_PASSIVE 16

/*
 * @brief Defines whether 'active LEDs' should be enabled.
 * Active LEDs react to a separate, configurable input (for example, door open signal).
 * Comment this line out to disable active LEDs.
 */
#define LED_ENABLE_ACTIVE

/*
 * @brief Defines whether 'passive LEDs' should be enabled.
 * Passive LEDs don't react to additional inputs like active LEDs.
 * Comment this line out to disable passive LEDs.
 */
#define LED_ENABLE_PASSIVE

/*
 * @brief Defines the LED control order of the 'active LEDs'. Requires ENABLE_ACTIVE_LEDS to be defined.
 */
#define LED_ORDER_ACTIVE GRB

/*
 * @brief Defines the LED control order of the 'passive LEDs'.
 */
#define LED_ORDER_PASSIVE GRB

/*
 * @brief Defines the digital pin to use for 'active LEDs'. Requires ENABLE_ACTIVE_LEDS to be defined.
 */
#define LED_PIN_ACTIVE PIN_PA7

/*
 * @brief Defines the digital pin to use for 'passive LEDs'.
 */
#define LED_PIN_PASSIVE PIN_PA6

/*
 * @brief Defines the LED type for the 'active LEDs'. Requires ENABLE_ACTIVE_LEDS to be defined.
 */
#define LED_TYPE_ACTIVE WS2812

/*
 * @brief Defines the LED type for the 'passive LEDs'.
 */
#define LED_TYPE_PASSIVE WS2812