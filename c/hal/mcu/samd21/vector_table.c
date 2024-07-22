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

/******************************************************************************
 * Local defines, types, and globals
 */

typedef void (*isr_handler_fn_t)(void);

/* https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html
 * "compiler generates function entry and exit sequences suitable
 * for use in an interrupt handler"
 */
#define ISR __attribute__ ((interrupt ("IRQ")))

/******************************************************************************
 * External Symbols
 */

/* system bootstrapper */
extern void hal_bootstrap ( void );

/* user code entry point - avoid including user-defined header */
extern int main ( void );

/******************************************************************************
 * Interrupt Service Routines
 */
ISR void isr_reset ( void )
{
  hal_bootstrap();
  main();
}

/******************************************************************************
 * ISR Vector Table
 * structure to overlay array of pointers to functions used to service
 * interrupts 
 * https://developerhelp.microchip.com/xwiki/bin/view/products/mcu-mpu/32bit-mcu/sam/samd21-mcu-overview/samd21-processor-overview/samd21-nvic-overview
 */

/* because this version is the bare minimum application to verify the 
 * development setup, the isr vector table isn't completely filled out,
 * meaning its size doesn't match the vector table on chip.
 * compute the size and hard code it for now
 */
#define VECTOR_TABLE_BOTTOM_ADDR  0x00000004 // lowest address of vector table
#define VECTOR_TABLE_TOP_ADDR     0x00000045 // highest address of vector table
#define VECTOR_TABLE_ENTRY_CNT ((VECTOR_TABLE_TOP_ADDR-VECTOR_TABLE_BOTTOM_ADDR)+1)/sizeof(uintptr_t)

/* set the section so the linker command script can place it correctly */
const isr_handler_fn_t isr_vector_table[VECTOR_TABLE_ENTRY_CNT] __attribute__ ((used, section(".isr_vector_table"))) = 
{
  /*0x00000004*/isr_reset,
  /*...*/
  /*0x000000AC*/
};

