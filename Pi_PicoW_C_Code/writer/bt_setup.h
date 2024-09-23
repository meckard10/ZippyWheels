#define HEARTBEAT_PERIOD_MS 100

int bt_init(void(*task)(void), uint16_t* message);
int bt_start();
