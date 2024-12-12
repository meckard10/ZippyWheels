////////////////////////////////////////////////////////////////////////////////
// Raspberry Pi Pico-W Bluetooth Reader Demo
// written by Travis Llado
// last edited 2023-11-08
////////////////////////////////////////////////////////////////////////////////

#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "stdio.h"

#include "bt_setup.h"

////////////////////////////////////////////////////////////////////////////////

#define PIN_PWM 21U
#define HALL_PAD 15
#define SCALING_FACTOR 65535/1295

uint16_t _received_duty_;
uint16_t _modified_duty_;
int pulseCount = 0;
int rpm = 0;

////////////////////////////////////////////////////////////////////////////////

void gpioIrqCallback(uint gpio, uint32_t event_mask){
	if ((event_mask & GPIO_IRQ_EDGE_RISE)> 0){
		pulseCount++; // Count the rising edges (voltage from hall sensor)
	}
}

void initialize_hall_sensor() {
   gpio_init(HALL_PAD);
   gpio_set_dir(HALL_PAD, GPIO_IN);
   gpio_pull_up(HALL_PAD);
   gpio_set_irq_enabled_with_callback(HALL_PAD, GPIO_IRQ_EDGE_RISE, true, gpioIrqCallback);
}

////////////////////////////////////////////////////////////////////////////////

void set_duty(void) {
  rpm = pulseCount*60/32; //32 pulses/revolutions * 60 seconds/min
  _modified_duty_ = _received_duty_;
  if (SCALING_FACTOR*(rpm-100) < _modified_duty_ ){
    _modified_duty_ += 10  ;
  }
  else if (SCALING_FACTOR*(rpm+100) > _modified_duty_ ){
    _modified_duty_ -= 10  ;
  }
  
  pwm_set_gpio_level(PIN_PWM, _modified_duty_);
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
    initialize_hall_sensor();
    bt_init(&set_duty, &_received_duty_);
    bt_start();
    
    while(true) {  
       // Do nothing, all further activity handled by bt handlers
       sleep_ms(1000);

       //printf("RPM: %d\n", rpm); 
	rpm=0;    
       pulseCount = 0;
       
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
