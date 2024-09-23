#define LED_QUICK_FLASH_DELAY_MS 100
#define LED_SLOW_FLASH_DELAY_MS 1000

int bt_init(void(*task)(void), uint16_t* message);
void bt_start();
