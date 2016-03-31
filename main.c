#include <stdint.h>
#include "driver/include/m2m_wifi.h"

// -------- Global Variables --------- //

// -------- Functions --------- //
static void wifi_cb(uint8_t u8MsgType, void *pvMsg)
{

}

int main(void) {
  // -------- Inits --------- //
  tstrWifiInitParam param;
  nm_bsp_init();
  m2m_memset((uint8*)&param, 0, sizeof(param));
  
  // ------ Event loop ------ //
  while (1) {

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
