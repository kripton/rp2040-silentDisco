extern "C" {
#include <stdio.h>
#include <hardware/clocks.h>
#include <hardware/gpio.h>

#include <pico/stdlib.h>

#include "pins.h"
#include "picotool_binary_information.h"

}

#include "log.h"

int main() {

    // SETUP COMPLETE
    LOG("SYSTEM: SETUP COMPLETE");

    while (true) {
        // TODO
        tight_loop_contents();
    }
};
