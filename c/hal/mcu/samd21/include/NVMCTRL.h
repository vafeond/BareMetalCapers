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

#if !defined __NVMCTRL_H__
#define __NVMCTRL_H__

#include <stdint.h>
#include "product_mapping.h"


/******************************************************************************
 * Memory Mappings
 */
#define NVM_BASE_ADDRESS 0x00000000
#define NVM_MAIN_OFFSET  0x00000000

#define NVM_CALIBRATION_AUX1_AREA1_ADDRESS  0x00806000
#define NVM_CALIBRATION_AUX1_AREA2_ADDRESS  0x00806008
#define NVM_CALIBRATION_AUX1_AREA3_ADDRESS  0x00806010
#define NVM_CALIBRATION_AUX1_AREA4_ADDRESS  0x00806020

#define NVMCTRL_ADDRESS (AHB_APB_BRIDGE_B_ADDRESS + NVMCTRL_OFFSET)


/******************************************************************************
 * NVM Software Calibration (Area 4)
 */
typedef struct {
  const volatile uint64_t :3;
  const volatile uint64_t :12;
  const volatile uint64_t :12;
  const volatile uint64_t ADC_LINEARITY:8;
  const volatile uint64_t ADC_BIASCAL:3;
  const volatile uint64_t OSC32K_CAL:7;
  const volatile uint64_t USB_TRANSN:5;
  const volatile uint64_t USB_TRANSP:5;
  const volatile uint64_t USB_TRIM:3;
  const volatile uint64_t DFLL48M_COARSE_CAL:6;
} NVM_SW_CAL_t;


/******************************************************************************
 * Helpers
 */
#define p_NVM_SW_CAL ((NVM_SW_CAL_t * const)(NVM_CALIBRATION_AUX1_AREA4_ADDRESS))
#define NVM_SW_CAL_READ(value) p_NVM_SW_CAL->value

#endif /* __NVMCTRL_H__ */
