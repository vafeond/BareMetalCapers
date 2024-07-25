/******************************************************************************
 *  This is free and unencumbered software released into the public domain.
 *
 *  Anyone is free to copy, modify, publish, use, compile, sell, or
 *  distribute this software, either in source code form or as a compiled
 *  binary, for any purpose, commercial or non-commercial, and by any
 *  means.
 *
 *  In jurisdictions that recognize copyright laws, the author or authors
 *  of this software dedicate any and all copyright interest in the
 *  software to the public domain. We make this dedication for the benefit
 *  of the public at large and to the detriment of our heirs and
 *  successors. We intend this dedication to be an overt act of
 *  relinquishment in perpetuity of all present and future rights to this
 *  software under copyright law.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *  OTHER DEALINGS IN THE SOFTWARE.
 *
 *  For more information, please refer to <https://unlicense.org>
 */

#include <stdint.h>
#include <stdbool.h>


/******************************************************************************
 * SAMD21
 */
#include "SYSCTRL.h"
#include "PORT.h"
#include "GCLK.h"
#include "NVMCTRL.h"
#include "USB.h"


/******************************************************************************
 * User Code
 */ 

USB_ENDPOINT_DESC_t usb_endpoints[8] = {0};
uint8_t usb_ep0_data_buffer[64];

void isr_usb ( void ) {
  // USB reset
  // USB_DEVICE_ADDRESS_SET(address)
}

int main ( void )
{
  uint32_t count = 0;

  /* setup the main clock */
  SYSCTR_OSC8M_8MHZ_SET;

  /* write DLL48M calibration data - NVM to SYSCTRL DFLLVAL */ 
  SYSCTR_DFLL_CAL_SET(COARSE,NVM_SW_CAL_READ(DFLL48M_COARSE_CAL));

  /* setup a generic clock generator to clock USB peripheral */
  GCLK_GENERATOR_48MHZ_ENABLE(GCLK_CLK_GENERATOR_1,GCLK_CLKCTRL_ID_USB);

  /* write USB calibration data - NVM to USB PADCAL */
  USB_CAL_SET(TRANSN,NVM_SW_CAL_READ(USB_TRANSN));
  USB_CAL_SET(TRANSP,NVM_SW_CAL_READ(USB_TRANSP));
  USB_CAL_SET(TRIM,NVM_SW_CAL_READ(USB_TRIM));

  /* Configure device mode */
  USB_MODE_DEVICE_SET

  /* Configure speed (CTRLB.SPDCONF) */
  USB_SPEED_SET(HIGH)

  /* enable USB (CTRLA.ENABLE = 1)  */
  USB_ENABLE
  USB_SYNCBUSY_ENABLE_SPIN

  /* Initialize endpoint descriptors */
  // ????????????
  USB_ENDPOINT_DESC_ADDRESS_SET(usb_endpoints);

  /* configure direction and type of endpoint */
  // ????????????
  USB_ENDPOINT_CONFIG_TYPE_IN_SET(0,DISABLED);
  USB_ENDPOINT_CONFIG_TYPE_OUT_SET(0,BULK);
  
  /* configure the endpoint data buffer size */
  // ????????????
  USB_ENDPOINT_DESC_SET(0,0,PCKSIZE.SIZE,USB_ENDPOINT_DESC_PCKSIZE_SIZE_64);

  /* configure the endpoint data buffer address */
  // ????????????
  USB_ENDPOINT_DESC_SET(0,0,ADDR,usb_ep0_data_buffer)

  /* 32.6.2 */





  /* OLD APP BELOW, KEEP AS WANT SOME VISUAL CLUE TOO */

  PORT_PIN_DIR_OUT(A,17);
  PORT_PIN_OUT_CLR(A,17);

  // #define TOGGLE_DELAY_COUNT 100000
  #define TOGGLE_DELAY_COUNT 30000
  while ( true )
  {
    if ( ++count >= TOGGLE_DELAY_COUNT )
    {
      // *PA_TOGGLE_OUTPUT = PA17_SET_MASK;
      PORT_PIN_OUT_TGL(A,17);
      count=0;
    }
  }

  return 0;
}

