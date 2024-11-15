// GPIO numbers of the RP2040 chip,
// NOT hardware pin numbers of the pico module!
// All in one file for easy reference

// I2C
// TODO: Verify!
#define PIN_I2C_SCL     5
#define PIN_I2C_SDA     4

// On-board LEDs
#define PIN_LEDA        6 // PWM 3A
#define PIN_LEDB        7 // PWM 3B
#define PIN_LEDC        8 // PWM 4A
#define PIN_LEDD        9 // PWM 4B
#define PIN_LEDE       10 // PWM 5A
#define PIN_LEDF       11 // PWM 5B
#define PIN_LEDG       12 // PWM 6A
#define PIN_LEDH       13 // PWM 6B

// Measurement input & output
#define PIN_SIG_IN     15 // PWM 7B
#define PIN_SIG_OUT    16 // PWM 0A

// Pico's on-board, single-color status LED
// If a regular "Pico" is detected at runtime => Port 25
#define PIN_LED_PICO   25
