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


/******************************************************************************
 * User Code
 */ 
int main ( void )
{
  uint32_t count = 0;

  /* setup the clock */
  SYSCTR_OSC8M_8MHZ_SET;
  GCLK_GENERATOR_48MHZ_ENABLE(GCLK_CLK_GENERATOR_1,GCLK_CLKCTRL_ID_USB);

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

