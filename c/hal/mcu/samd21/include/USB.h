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

#if !defined __USB_H__
#define __USB_H__

#include <stdint.h>
#include "product_mapping.h"


/******************************************************************************
 * Memory Mappings
 */
#define USB_ADDRESS (AHB_APB_BRIDGE_B_ADDRESS + USB_OFFSET)

// Dependencies:
// - [DONE] IO Lines multiplexed for USB
//   - [DONE] PORT.h
//     - PORT_PERIPHERAL_USB_ENABLE
//     - PORT_PERIPHERAL_USB_DISABLE
// - [DEFERRED] Power Management
//   - Idle and sleep modes, defer
// - [TODO] Clocks
//   - [DONE] PM
//     - PM CLK_USB_APB enabled by default
//     - PM PM_AHBMASK_t.USB is set by default
//   - [DONE] CLK_USB_AHB (MCLK), 8MHz
//     - SYSCTRL.OSC8M enabled, set to 8MHz (clear PRESC)?
//   - [DONE] GCLK_USB (GCLK), 48MHz
//     - GCLK_GENERATOR_48MHZ_ENABLE
//   - [DONE] Synchronization 
//     - "writes to certain registers will require synchonization between clock domains"]}
//     - Applies to
//       - GCLK.GENCTRL
//       - GCLK.GENDIV
//       - CTRL (GCLK?)
//   - [TODO] "USB Clock Recovery Module"]
//     - no link provided, doesn't appear again when searching doc
//
// USB Module:
// - 
//

/******************************************************************************
 * Common
 */

/* Control B */
typedef struct {
  volatile uint16_t DETACH:1;
  volatile uint16_t UPRSM:1;
  volatile uint16_t SPDCONF:2;
  volatile uint16_t NREPLY:1;
  volatile uint16_t TSTJ:1;
  volatile uint16_t TSTK:1;
  volatile uint16_t TSTPCKT:1;
  volatile uint16_t OPMODE2:1;
  volatile uint16_t GNAK:1;
  volatile uint16_t LPMHDSK:2;
  volatile uint16_t :4;
} USB_CTRLB_t;

/* Device Address */
typedef struct {
  volatile uint8_t DADD:7;
  volatile uint8_t ADDEN:1;
} USB_DADD_t;

/* Status */
typedef enum USB_STATUS_SPEED_t:uint8_t {
  USB_STATUS_SPEED_LOW = 0x0,
  USB_STATUS_SPEED_FULL = 0x1,
  USB_STATUS_SPEED_HIGH = 0x2
} USB_STATUS_SPEED_t;
typedef enum USB_STATUS_LINESTATUS_t:uint8_t {
  USB_STATUS_LINESTATUS_SE0RESET = 0x0,
  USB_STATUS_LINESTATUS_FSJ_OR_LSK = 0x1,
  USB_STATUS_LINESTATUS_FSK_OR_LSJ = 0x2
} USB_STATUS_LINESTATUS_t;
typedef struct {
  volatile uint8_t                        :2;
  volatile USB_STATUS_SPEED_t             SPEED:2;
  volatile uint8_t                        :2;
  const volatile USB_STATUS_LINESTATUS_t  LINESTATE:2;
} USB_STATUS_t;

/* Device Frame Number */
typedef struct {
  volatile uint16_t MFNUM:2;
  volatile uint16_t FNUM:11;
  volatile uint16_t :1;
  volatile uint16_t FNCERR:1;
} USB_FNUM_t;

/* Device Interrupt Enable Set/Clear/Status and Clear */
typedef struct {
  volatile uint16_t SUSPEND:1;
  volatile uint16_t MSOF:1;
  volatile uint16_t SOF:1;
  volatile uint16_t EORST:1;
  volatile uint16_t WAKEUP:1;
  volatile uint16_t EORSM:1;
  volatile uint16_t UPRSM:1;
  volatile uint16_t RAMACER:1;
  volatile uint16_t LPMNYET:1;
  volatile uint16_t LPMSUSP:1;
  volatile uint16_t :6;
} USB_INT_t;

/* Endpoint Interrupt Summary */
typedef struct {
  volatile uint16_t EPINT0:1;
  volatile uint16_t EPINT1:1;
  volatile uint16_t EPINT2:1;
  volatile uint16_t EPINT3:1;
  volatile uint16_t EPINT4:1;
  volatile uint16_t EPINT5:1;
  volatile uint16_t EPINT6:1;
  volatile uint16_t EPINT7:1;
  volatile uint16_t :8;
} USB_EPINTSMRY_t;

#define USB_COMMON_CTRLB_OFFSET 0x08
#define USB_COMMON_DADD_OFFSET  0x0A
#define USB_COMMON_STATUS       0x0C
#define USB_COMMON_FNUM         0x10
#define USB_COMMON_INTENCLR     0x14
#define USB_COMMON_INTENSET     0x18
#define USB_COMMON_INTFLAG      0x1C
#define USB_COMMON_EPINTSMRY    0x20


/******************************************************************************
 * Device
 */

/* Device Endpoint Configuration */
typedef struct {
  volatile uint8_t EPTYPE0:3;
  volatile uint8_t :1;
  volatile uint8_t EPTYPE1:3;
  volatile uint8_t NYETDIS:1;
} USB_EPCFG_t;

/******************************************************************************
 * Host
 */
/* TBD */

#endif /* __USB_H__ */
