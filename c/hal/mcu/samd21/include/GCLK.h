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

#if !defined __GCLK_H__
#define __GCLK_H__

#include <stdint.h>
#include "product_mapping.h"


/******************************************************************************
 * Memory Mappings
 */
#define GCLK_ADDRESS (AHB_APB_BRIDGE_A_ADDRESS + GCLK_OFFSET)

/******************************************************************************
 * Common
 */
typedef enum GCLK_CLK_GEN:uint8_t {
  GCLK_CLK_GENERATOR_0 = 0x00,
  GCLK_CLK_GENERATOR_1 = 0x01,
  GCLK_CLK_GENERATOR_2 = 0x02,
  GCLK_CLK_GENERATOR_3 = 0x03,
  GCLK_CLK_GENERATOR_4 = 0x04,
  GCLK_CLK_GENERATOR_5 = 0x05,
  GCLK_CLK_GENERATOR_6 = 0x06,
  GCLK_CLK_GENERATOR_7 = 0x07,
  GCLK_CLK_GENERATOR_8 = 0x08
} GCLK_CLK_GEN_t;

/******************************************************************************
 * Control
 */
typedef struct {
  volatile uint8_t SWRST:1;
  volatile uint8_t :7;
} GCLK_CTRL_t;

/******************************************************************************
 * Status
 */
typedef struct {
  volatile uint8_t :7;
  volatile uint8_t SYNCBUSY:1;
} GCLK_STATUS_t;

/******************************************************************************
 * Generic Clock Control
 */
typedef enum GCLK_CLKCTRL_ID:uint8_t {
  /* ... */
  GCLK_CLKCTRL_ID_USB = 0x06
  /* ... */
} GCLK_CLKCTRL_ID_t;

typedef struct {
  volatile GCLK_CLKCTRL_ID_t  ID:6;
  volatile uint16_t           :2;
  volatile GCLK_CLK_GEN_t     GEN:4;
  volatile uint16_t           :2;
  volatile uint16_t           CLKEN:1;
  volatile uint16_t           WRTLOCK:1;
} GCLK_CLKCTRL_t;

/******************************************************************************
 * Generic Clock Generator Control
 */
typedef enum GCLK_GENCTRL_SRC:uint8_t {
  GCLK_GENCTRL_SRC_XOSC       = 0x00,
  GCLK_GENCTRL_SRC_GCLKIN     = 0x01,
  GCLK_GENCTRL_SRC_GCLKGEN1   = 0x02,
  GCLK_GENCTRL_SRC_OSCULP32K  = 0x03,
  GCLK_GENCTRL_SRC_OSC32K     = 0x04,
  GCLK_GENCTRL_SRC_XOSC32K    = 0x05,
  GCLK_GENCTRL_SRC_OSC8M      = 0x06,
  GCLK_GENCTRL_SRC_DFLL48M    = 0x07,
  GCLK_GENCTRL_SRC_FDPLL96M   = 0x08
} GCLK_GENCTRL_SRC_t;

typedef struct {
  volatile GCLK_CLK_GEN_t     ID:4;
  volatile uint32_t           :4;
  volatile GCLK_GENCTRL_SRC_t SRC:5;
  volatile uint32_t           :3;
  volatile uint32_t           GENEN:1;
  volatile uint32_t           IDC:1;
  volatile uint32_t           OOV:1;
  volatile uint32_t           OE:1;
  volatile uint32_t           DIVSEL:1;
  volatile uint32_t           RUNSTDBY:1;
  volatile uint32_t :10;
} GCLK_GENCTRL_t;

/******************************************************************************
 * Generic Clock Generator Division
 */
typedef struct {
  volatile GCLK_CLK_GEN_t ID:4;
  volatile uint32_t       :4;
  volatile uint32_t       DIV:16;
  volatile uint32_t       :8;
} GCLK_GENDIV_t;

/******************************************************************************
 * Generic Clock Controller
 */
typedef struct {
  GCLK_CTRL_t     CTRL;
  GCLK_STATUS_t   STATUS;
  GCLK_CLKCTRL_t  CLKCTRL;
  GCLK_GENCTRL_t  GENCTRL;
  GCLK_GENDIV_t   GENDIV;
} GCLK_t;

/******************************************************************************
 * Helpers
 */

#define GCLK_SYNCWAIT_SPIN {  \
  while( 1 == ((GCLK_t*)GCLK_ADDRESS)->STATUS.SYNCBUSY ) {} \
}

/*
 * generator, e.g. GCLK_CLK_GENERATOR_1
 * peripheral, e.g. GCLK_CLKCTRL_ID_USB
 */
#define GCLK_GENERATOR_48MHZ_ENABLE(generator,peripheral) { \
  GCLK_GENDIV_t gendiv={0};                                 \
  GCLK_GENCTRL_t genctrl={0};                               \
  GCLK_CLKCTRL_t clkctrl={0};                               \
  gendiv.ID=generator;                                      \
  gendiv.DIV=0;                                             \
  GCLK_SYNCWAIT_SPIN;                                       \
  ((GCLK_t*)GCLK_ADDRESS)->GENDIV=gendiv;                   \
  genctrl.ID=generator;                                     \
  genctrl.SRC=GCLK_GENCTRL_SRC_DFLL48M;                     \
  genctrl.GENEN=1;                                          \
  GCLK_SYNCWAIT_SPIN;                                       \
  ((GCLK_t*)GCLK_ADDRESS)->GENCTRL=genctrl;                 \
  clkctrl.ID=peripheral;                                    \
  clkctrl.GEN=generator;                                    \
  clkctrl.CLKEN=1;                                          \
  GCLK_SYNCWAIT_SPIN;                                       \
  ((GCLK_t*)GCLK_ADDRESS)->CLKCTRL=clkctrl;                 \
}
  




#endif /* __GCLK_H__ */
