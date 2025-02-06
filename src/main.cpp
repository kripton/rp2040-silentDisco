extern "C" {
#include <stdio.h>
#include <stdint.h>
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#include "pico/stdlib.h"

#include "pins.h"
#include "picotool_binary_information.h"

}

#include "pdmAudio.h"

#include "log.h"

// Super-globals (for all modules)
Log logger;

int main() {
    // Set 
    set_sys_clock_khz(115200, true);

    stdio_init_all();

    // /!\ Do NOT use LOG() above this line! /!\

    logger.init();

    // Set up all LEDs as PWMs and outputs
    gpio_init(PIN_LED_A);
    gpio_init(PIN_LED_B);
    gpio_init(PIN_LED_C);
    gpio_init(PIN_LED_D);
    gpio_init(PIN_LED_E);
    gpio_init(PIN_LED_F);
    gpio_init(PIN_LED_G);
    gpio_init(PIN_LED_H);
    gpio_set_dir(PIN_LED_A, true);
    gpio_set_dir(PIN_LED_B, true);
    gpio_set_dir(PIN_LED_C, true);
    gpio_set_dir(PIN_LED_D, true);
    gpio_set_dir(PIN_LED_E, true);
    gpio_set_dir(PIN_LED_F, true);
    gpio_set_dir(PIN_LED_G, true);
    gpio_set_dir(PIN_LED_H, true);


    gpio_put(PIN_LED_A, 1);

    // PDM object
    pdmAudio pdm;

    pdm.begin(14); // Left channel

    // SETUP COMPLETE
    LOG("SYSTEM: SETUP COMPLETE");

    while (true) {
  pdm.tone(262, .25);
  pdm.tone(0, .165);

  pdm.tone(196, 0.125);
  pdm.tone(0, .08);

  pdm.tone(196, 0.125);
  pdm.tone(0, .08);

  pdm.tone(220, 0.25);
  pdm.tone(0, .16);

  pdm.tone(196, 0.25);
  pdm.tone(0, .16);
  
  pdm.tone(0, 0.25);
  pdm.tone(0, .16);
  
  pdm.tone(247, 0.25);
  pdm.tone(0, .16);

  pdm.tone(262, 0.25);
  pdm.tone(0, 1);

        sleep_us(10);
    }
};
