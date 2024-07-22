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

#if !defined __PM_H__
#define __PM_H__

#include <stdint.h>
#include "product_mapping.h"


/******************************************************************************
 * Memory Mappings
 */
#define PM_ADDRESS (AHB_APB_BRIDGE_A_ADDRESS + PM_OFFSET)

enum PM_DIV {
  PM_CPUSEL_CPUDIV_DIV1 = 0x00,
  PM_CPUSEL_CPUDIV_DIV2 = 0x01,
  PM_CPUSEL_CPUDIV_DIV4 = 0x02,
  PM_CPUSEL_CPUDIV_DIV8 = 0x03,
  PM_CPUSEL_CPUDIV_DIV16 = 0x04,
  PM_CPUSEL_CPUDIV_DIV32 = 0x05,
  PM_CPUSEL_CPUDIV_DIV64 = 0x06,
  PM_CPUSEL_CPUDIV_DIV128 = 0x07
};

/******************************************************************************
 * Control
 */
typedef struct {
  const volatile uint8_t :8;
} PM_CTRL_t;

/******************************************************************************
 * Sleep Mode
 */
typedef struct {
  volatile uint8_t IDLE:2;
  volatile uint8_t :6;
} PM_SLEEP_t;

/******************************************************************************
 * CPU Clock Select
 */
typedef struct {
  volatile uint8_t CPUDIV:3;
  volatile uint8_t :5;
} PM_CPUSEL_t;

/******************************************************************************
 * APBA Clock Select
 */
typedef struct {
  volatile uint8_t APBADIV:3;
  volatile uint8_t :5;
} PM_APBASEL_t;

/******************************************************************************
 * APBB Clock Select
 */
typedef struct {
  volatile uint8_t APBBDIV:3;
  volatile uint8_t :5;
} PM_APBBSEL_t;

/******************************************************************************
 * APBC Clock Select
 */
typedef struct {
  volatile uint8_t APBCDIV:3;
  volatile uint8_t :5;
} PM_APBCSEL_t;

/******************************************************************************
 * AHB Mask
 */
typedef struct {
  volatile uint32_t HPB0:1;
  volatile uint32_t HPB1:1;
  volatile uint32_t HPB2:1;
  volatile uint32_t DSU:1;
  volatile uint32_t NVMCTRL:1;
  volatile uint32_t DMAC:1;
  volatile uint32_t USB:1;
  volatile uint32_t :25;
} PM_AHBMASK_t;

/******************************************************************************
 * APBA Mask
 */
typedef struct {
  volatile uint32_t PAC0:1;
  volatile uint32_t PM:1;
  volatile uint32_t SYSCTRL:1;
  volatile uint32_t GCLK:1;
  volatile uint32_t WDT:1;
  volatile uint32_t RTC:1;
  volatile uint32_t EIC:1;
  volatile uint32_t :25;
} PM_APBAMASK_t;

/******************************************************************************
 * APBB Mask
 */
typedef struct {
  volatile uint32_t PAC1:1;
  volatile uint32_t DSU:1;
  volatile uint32_t NVMCTRL:1;
  volatile uint32_t PORT:1;
  volatile uint32_t DMAC:1;
  volatile uint32_t USB:1;
  volatile uint32_t :26;
} PM_APBBMASK_t;

/******************************************************************************
 * APBC Mask
 */
typedef struct {
  volatile uint32_t PAC2:1;
  volatile uint32_t EVSYS:1;
  volatile uint32_t SERCOM0:1;
  volatile uint32_t SERCOM1:1;
  volatile uint32_t SERCOM2:1;
  volatile uint32_t SERCOM3:1;
  volatile uint32_t SERCOM4:1;
  volatile uint32_t SERCOM5:1;
  volatile uint32_t TCC0:1;
  volatile uint32_t TCC1:1;
  volatile uint32_t TCC2:1;
  volatile uint32_t TC3:1;
  volatile uint32_t TC4:1;
  volatile uint32_t TC5:1;
  volatile uint32_t TC6:1;
  volatile uint32_t TC7:1;
  volatile uint32_t ADC:1;
  volatile uint32_t AC:1;
  volatile uint32_t DAC:1;
  volatile uint32_t PTC:1;
  volatile uint32_t I2S:1;
  volatile uint32_t AC1:1;
  volatile uint32_t :2;
  volatile uint32_t TCC3:1;
  volatile uint32_t :7;
} PM_APBCMASK_t;

/******************************************************************************
 * Interrupt Enable Clear 
 */
typedef struct {
  volatile uint8_t CKRDY:1;
  volatile uint8_t :7;
} PM_INTENCLR_t;

/******************************************************************************
 * Interrupt Enable Set
 */
typedef struct {
  volatile uint8_t CKRDY:1;
  volatile uint8_t :7;
} PM_INTENSET_t;

/******************************************************************************
 * Interrupt Flag Status and Clear
 */
typedef struct {
  volatile uint8_t CKRDY:1;
  volatile uint8_t :7;
} PM_INTFLAG_t;

/******************************************************************************
 * Reset Cause
 */
typedef struct {
  volatile uint8_t POR:1;
  volatile uint8_t BOD12:1;
  volatile uint8_t BOD33:1;
  volatile uint8_t :1;
  volatile uint8_t EXT:1;
  volatile uint8_t WDT:1;
  volatile uint8_t SYST:1;
  volatile uint8_t :1;
} PM_RCAUSE_t;

#endif /* __PM_H__ */
