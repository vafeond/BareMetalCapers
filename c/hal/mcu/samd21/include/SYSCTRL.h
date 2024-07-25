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

#if !defined __SYSCTRL_H__
#define __SYSCTRL_H__

#include <stdint.h>
#include "product_mapping.h"


/******************************************************************************
 * 8MHz Internal Oscillator (OSC8M) Control
 */
typedef struct {
  uint32_t :1;                    /* 0: unused */
  volatile uint32_t ENABLE :1;    /* default: 1 */
  uint32_t :4;                    /* 2-5: unused */
  volatile uint32_t RUNSTDBY:1;   /* default: 0 */
  volatile uint32_t ONDEMAND:1;   /* default: 1*/
  volatile uint32_t PRESC:2;      /* default: 0x3 (divide by eight) */
  uint32_t :6;                    /* 10-15: unused */
  const uint32_t CALIB_TEMP:6;    /* written during manufacture, protecting from overwrite */
  const uint32_t CALIB_OVERALL:6; /* written during manufacture, protecting from overwrite */
  uint32_t :2;                    /* 28-29: unused */
  const uint32_t FRANGE:2;        /* written during manufacture, protecting from overwrite */
} SYSCTRL_OSC8M_t;

enum SYSCTRL_OSC8M_Prescaler {
  SYSCTRL_OSC8M_PRESCALER_DIVIDE_BY_ONE   = 0x0,
  SYSCTRL_OSC8M_PRESCALER_DIVIDE_BY_TWO   = 0x1,
  SYSCTRL_OSC8M_PRESCALER_DIVIDE_BY_FOUR  = 0x2,
  SYSCTRL_OSC8M_PRESCALER_DIVIDE_BY_EIGHT = 0x3  /* default */
};

#define SYSCTRL_OSC8M_OFFSET 0x20

#define p_SYSCTRL_OSC8M ((SYSCTRL_OSC8M_t * const)(AHB_APB_BRIDGE_A_ADDRESS + SYSCTRL_OFFSET + SYSCTRL_OSC8M_OFFSET))

#define SYSCTR_OSC8M_8MHZ_SET p_SYSCTRL_OSC8M->PRESC=SYSCTRL_OSC8M_PRESCALER_DIVIDE_BY_ONE;
#define SYSCTR_OSC8M_4MHZ_SET p_SYSCTRL_OSC8M->PRESC=SYSCTRL_OSC8M_PRESCALER_DIVIDE_BY_TWO;
#define SYSCTR_OSC8M_2MHZ_SET p_SYSCTRL_OSC8M->PRESC=SYSCTRL_OSC8M_PRESCALER_DIVIDE_BY_FOUR;
#define SYSCTR_OSC8M_1MHZ_SET p_SYSCTRL_OSC8M->PRESC=SYSCTRL_OSC8M_PRESCALER_DIVIDE_BY_EIGHT;


/******************************************************************************
 * DFLL48M Value
 */
typedef struct {
  volatile uint32_t FINE:10;
  volatile uint32_t COARSE:6;
  const volatile uint32_t DIFF:16;
} SYSCTRL_DFLLVAL_t;

#define SYSCTRL_DFLLVAL_OFFSET 0x28

#define p_SYSCTRL_DFLLVAL ((SYSCTRL_DFLLVAL_t * const)(AHB_APB_BRIDGE_A_ADDRESS + SYSCTRL_OFFSET + SYSCTRL_DFLLVAL_OFFSET))

#define SYSCTR_DFLL_CAL_SET(calval,value) p_SYSCTRL_DFLLVAL->calval=value;

#endif /* __SYSCTRL_H__ */
