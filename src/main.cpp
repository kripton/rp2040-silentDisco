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

#include "log.h"

// Super-globals (for all modules)
Log logger;

uint16_t ledLevel[8] = {0};
volatile uint64_t lastInRisingTime;
uint16_t level = 0;
double inFreq = 0;

// Interrupt service handler for SIGNAL INPUT
void __isr gpio_callback(uint gpio, uint32_t events) {
    uint64_t timeDiff = 0;

    if (events == GPIO_IRQ_EDGE_RISE) {
        gpio_put(PIN_LED_H, 1);
        timeDiff = time_us_64() - lastInRisingTime;

        inFreq = (1.0f / timeDiff) * 1000 * 1000;
        if (inFreq > 300) {
            inFreq = 300;
        }

        lastInRisingTime = time_us_64();
    } else if (events == GPIO_IRQ_EDGE_FALL) {
        gpio_put(PIN_LED_H, 0);
    }
}

int main() {

    // Underclock the board to 50MHz for lower PWM frequencies
    set_sys_clock_khz(50000, true);

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


    // Set up the input IRQ for the SIGNAL INPUT
    lastInRisingTime = time_us_64();
    gpio_init(PIN_SIG_IN);
    gpio_pull_down(PIN_SIG_IN);
    gpio_set_irq_enabled_with_callback(PIN_SIG_IN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // Set up the PWM for the output
    pwm_config config = pwm_get_default_config();
    // "PWM config is free-running at system clock speed, no phase correction, wrapping at 0xffff, with standard polarities for channels A and B."
    pwm_config_set_clkdiv(&config, 4.f); // 250 MHz / 4 = 62.5MHz
                                         // Wrapping at 0xffff => 953,6888685435264 Hz
    // Requirement is:
    // Duty cycle is 50%
    // fin = 0,5Hz to 30Hz
    // fout = fin * 10
    // fout = 5Hz to 300Hz
    // From datasheet:
    // fPWM = fsys / ((TOP + 1) * (CSR_PH_CORRECT + 1) * (DIV_INT + DIV_FRAC/16))
    // fPWM = 250000000 / ((65536) * 1 * 250) = 15,2587890625 Hz
    // fPWM = 100000000 / ((65536) * 1 * 255) = 5,983838848039216 Hz
    // fPWM = 50000000 / ((65536) * 1 * 255) = 2,991919424019608 Hz
    // fPWM = 50000000 / ((65536) * 1 * 1) = 762,939453125 Hz
    pwm_init(0, &config, false);
    pwm_set_gpio_level(PIN_SIG_OUT, 0);
    gpio_set_function(PIN_SIG_OUT, GPIO_FUNC_PWM);

    // For DEBUG
    gpio_set_function(25, GPIO_FUNC_PWM);

    gpio_put(PIN_LED_A, 1);

    // SETUP COMPLETE
    LOG("SYSTEM: SETUP COMPLETE");

    while (true) {
        if ((time_us_64() - lastInRisingTime) > (uint64_t)3 * 1000 * 1000) {
            gpio_put(PIN_LED_B, 0);
            LOG("TIMEOUT. Output disabled! Last InFreq: %f Hz", inFreq);
            pwm_set_gpio_level(PIN_SIG_OUT, 0);
            pwm_set_enabled(0, false);
            level = 0;
        } else {
            gpio_put(PIN_LED_B, 1);
            // 50% duty cycle, variable frequency
            pwm_set_gpio_level(PIN_SIG_OUT, 32768);
            // Calculate and set the correct frequency of the PWM
            float divider = (double)clock_get_hz(clk_sys) / (inFreq * 10.0f * 65536.0f);
            double outFreq = (double)clock_get_hz(clk_sys) / (65536.0f * divider);
            LOG("IN FREQ: %f Hz. Divider: %f. OutFreq: %f Hz", inFreq, divider, outFreq);
            pwm_set_clkdiv(0, divider);
            pwm_set_enabled(0, true);
            level = level + 5;
        }

        // Eye candy!
        for (int i = PIN_LED_A; i <= PIN_LED_H; i++) {
            pwm_set_gpio_level(i, level + (UINT16_MAX / 8));
        }

        sleep_us(10);
    }
};
