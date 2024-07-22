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
 * RedBoard Turbo
 * Blue LED: PA17
 */

/* Product Mapping: AHB-APB bridges */ 
#define AHB_APB_BRIDGE_A 0x40000000
#define AHB_APB_BRIDGE_B 0x41000000
#define AHB_APB_BRIDGE_C 0x42000000

/* Product Mapping: PORT offset */ 
#define PORT 0x4400

/* PORT configuration and control offsets */
#define DIR 0x00
#define DIRCLR 0x04 
#define DIRSET 0x08 
#define DIRTGL 0x0C /* toggle direction */

#define OUT 0x10 
#define OUTCLR 0x14 
#define OUTSET 0x18
#define OUTTGL 0x1C /* toggle output (hi/low) */

/* PA helper macros */
#define PA_CONFIG_OUTPUT (uint32_t*)(AHB_APB_BRIDGE_B + PORT + DIRSET)
#define PA_CONFIG_INPUT (uint32_t*)(AHB_APB_BRIDGE_B + PORT + DIRCLR)

#define PA_CLR_OUTPUT (uint32_t*)(AHB_APB_BRIDGE_B + PORT + OUTCLR)
#define PA_TOGGLE_OUTPUT (uint32_t*)(AHB_APB_BRIDGE_B + PORT + OUTTGL)

/* set bit 17 for blue LED */
#define PA17_SET_MASK 0x1<<17


/******************************************************************************
 * User Code
 */ 
int main ( void )
{
  uint32_t count = 0;

  *PA_CONFIG_OUTPUT = PA17_SET_MASK;
  *PA_CLR_OUTPUT = PA17_SET_MASK;

  // #define TOGGLE_DELAY_COUNT 50000
  #define TOGGLE_DELAY_COUNT 20000
  while ( true )
  {
    if ( ++count >= TOGGLE_DELAY_COUNT )
    {
      *PA_TOGGLE_OUTPUT = PA17_SET_MASK;
      count=0;
    }
  }

  return 0;
}

