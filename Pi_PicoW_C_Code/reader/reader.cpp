////////////////////////////////////////////////////////////////////////////////
// Raspberry Pi Pico-W Bluetooth Reader Demo
// written by Travis Llado
// last edited 2023-11-08
////////////////////////////////////////////////////////////////////////////////

#include "hardware/pwm.h"
#include "pico/stdlib.h"

#include "bt_setup.h"

////////////////////////////////////////////////////////////////////////////////

#define PIN_PWM 21U

uint16_t _received_duty_;

////////////////////////////////////////////////////////////////////////////////

void set_duty(void) {
  pwm_set_gpio_level(PIN_PWM, _received_duty_);
 }

////////////////////////////////////////////////////////////////////////////////

void init_pwm() {
  gpio_set_function(PIN_PWM, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(PIN_PWM);
  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, 40.0); //frequency
  pwm_init(slice_num, &config, true);
  pwm_set_gpio_level(PIN_PWM, 0U);
}

////////////////////////////////////////////////////////////////////////////////

int main() {
    stdio_init_all();

    init_pwm();

    bt_init(&set_duty, &_received_duty_);
    bt_start();

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
