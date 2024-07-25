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

#include <assert.h>
#if !defined __USB_H__
#define __USB_H__

#include <stdint.h>
#include "product_mapping.h"


/******************************************************************************
 * Memory Mappings
 */
#define USB_ADDRESS (AHB_APB_BRIDGE_B_ADDRESS + USB_OFFSET)

// - [TODO] "USB Clock Recovery Module"]
//   - no link provided, doesn't appear again when searching doc
// - [TODO] Interrupts
//   - USB NVIC 7

/******************************************************************************
 * Common
 */

/* Control B */
typedef enum USB_CTRLA_SPDCONF_SPEED_t:uint8_t{
  USB_CTRLA_SPDCONF_SPEED_FULL = 0x0,
  USB_CTRLA_SPDCONF_SPEED_LOW = 0x1,
  USB_CTRLA_SPDCONF_SPEED_HIGH = 0x2,
  USB_CTRLA_SPDCONF_SPEED_HIGH_TEST = 0x3
} USB_CTRLA_SPDCONF_SPEED_t;
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

#define USB_CTRLB_OFFSET 0x08

#define p_USB_CTRLB ((USB_CTRLB_t * const)(USB_ADDRESS + USB_CTRLB_OFFSET))

#define USB_SPEED_SET(speed) p_USB_CTRLB->SPDCONF=USB_CTRLA_SPDCONF_SPEED_##speed;

#define USB_HOST_ATTACH p_USB_CTRLB->DETACH=0;
#define USB_HOST_DETACH p_USB_CTRLB->DETACH=1;

/* Device Address */
typedef struct {
  volatile uint8_t DADD:7;
  volatile uint8_t ADDEN:1;
} USB_DADD_t;

#define USB_DADD_OFFSET 0x0A

#define p_USB_DADD ((USB_DADD_t * const)(USB_ADDRESS + USB_DADD_OFFSET))

#define USB_DEVICE_ADDRESS_SET(address) p_USB_DADD->DADD=address;p_USB_DADD->ADDEN=1;
#define USB_DEVICE_ADDRESS_CLEAR(address) p_USB_DADD->ADDEN=0;

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

/* Device Endpoint Descriptor */
typedef enum USB_ENDPOINT_DESC_PCKSIZE_SIZE_t:uint8_t {
  USB_ENDPOINT_DESC_PCKSIZE_SIZE_8 = 0x0,
  USB_ENDPOINT_DESC_PCKSIZE_SIZE_16 = 0x1,
  USB_ENDPOINT_DESC_PCKSIZE_SIZE_32 = 0x2,
  USB_ENDPOINT_DESC_PCKSIZE_SIZE_64 = 0x3,
  /* below isochronous mode only */
  USB_ENDPOINT_DESC_PCKSIZE_SIZE_128 = 0x4,
  USB_ENDPOINT_DESC_PCKSIZE_SIZE_256 = 0x5,
  USB_ENDPOINT_DESC_PCKSIZE_SIZE_512 = 0x6,
  USB_ENDPOINT_DESC_PCKSIZE_SIZE_1023 = 0x7
} USB_ENDPOINT_DESC_PCKSIZE_SIZE_t;
typedef struct __attribute__((packed)) {
  volatile uint32_t                         BYTE_COUNT:14;
  volatile uint32_t                         MULTI_PACKET_SIZE:14;
  volatile USB_ENDPOINT_DESC_PCKSIZE_SIZE_t SIZE:3;
  volatile uint32_t                         AUTO_ZLP:1;
} USB_ENDPOINT_DESC_PCKSIZE_t;
typedef struct __attribute__((packed)) {
  volatile uint32_t SUBPID:4;
  volatile uint32_t VARIABLE:11;
  volatile uint32_t :1;
  /* manual padding ensuring USB_ENDPOINT_DESC_t is correct size */
  volatile uint32_t :16; 
} USB_ENDPOINT_DESC_EXTREG_t;
typedef struct __attribute__((packed)) {
  volatile uint8_t CDCERR:1;
  volatile uint8_t ERRORFLOW:1;
} USB_ENDPOINT_DESC_STATUS_BK_t;
typedef struct __attribute__((packed)) {
  /* bank 0 */
  volatile uint8_t*             B0_ADDR;       /* Address of data buffer */
  USB_ENDPOINT_DESC_PCKSIZE_t   B0_PCKSIZE;
  USB_ENDPOINT_DESC_EXTREG_t    B0_EXTREG;     
  USB_ENDPOINT_DESC_STATUS_BK_t B0_STATUS_BK;
  volatile uint32_t             :24;

  /* bank 1 */
  volatile uint8_t*             B1_ADDR;       /* Address of data buffer */
  USB_ENDPOINT_DESC_PCKSIZE_t   B1_PCKSIZE;
  volatile uint32_t             :32;
  USB_ENDPOINT_DESC_STATUS_BK_t B1_STATUS_BK;
  volatile uint32_t             :24;
} USB_ENDPOINT_DESC_t;
static_assert ( sizeof(USB_ENDPOINT_DESC_t) == 32 );

#define USB_DESCADD_OFFSET 0x24
#define pp_USB_DESCADD ((USB_ENDPOINT_DESC_t ** const)(USB_ADDRESS + USB_DESCADD_OFFSET))
#define USB_ENDPOINT_DESC_ADDRESS_SET(address) (*pp_USB_DESCADD)=address; 

#define USB_ENDPOINT_DESC_SET(n,bank,member,value) (*pp_USB_DESCADD)[n].B##bank##_##member=value;


/* Device Endpoint Configuration */
typedef enum USB_EPCFG_EPTYPE_t:uint8_t{
  USB_EPCFG_EPTYPE_DISABLED     = 0x0,
  USB_EPCFG_EPTYPE_CONTROL      = 0x1,
  USB_EPCFG_EPTYPE_ISOCHRONOUS  = 0x2,
  USB_EPCFG_EPTYPE_BULK         = 0x3,
  USB_EPCFG_EPTYPE_INTERRUPT    = 0x4,
  USB_EPCFG_EPTYPE_DUALBANK     = 0x5
} USB_EPCFG_EPTYPE_t;
typedef struct {
  volatile USB_EPCFG_EPTYPE_t EPTYPE0:3;
  volatile uint8_t            :1;
  volatile USB_EPCFG_EPTYPE_t EPTYPE1:3;
  volatile uint8_t            NYETDIS:1;
} USB_EPCFG_t;

#define USB_EPCFG_OFFSET 0x100

#define p_USB_EPCFG(n) ((USB_EPCFG_t * const)(USB_ADDRESS + USB_EPCFG_OFFSET +(n * 0x20)))

#define USB_ENDPOINT_CONFIG_TYPE_IN_SET(n,type) p_USB_EPCFG(n)->EPTYPE0=USB_EPCFG_EPTYPE_##type;
#define USB_ENDPOINT_CONFIG_TYPE_OUT_SET(n,type) p_USB_EPCFG(n)->EPTYPE1=USB_EPCFG_EPTYPE_##type;

/******************************************************************************
 * Host
 */

/* Control A */
typedef struct {
  volatile uint8_t SWRST:1;
  volatile uint8_t ENABLE:1;
  volatile uint8_t RUNSTDBY:1;
  volatile uint8_t :4;
  volatile uint8_t MODE:1;
} USB_CTRLA_t;

#define USB_CTRLA_OFFSET 0x00

#define p_USB_CTRLA ((USB_CTRLA_t * const)(USB_ADDRESS + USB_CTRLA_OFFSET))

#define USB_RESET (p_USB_CTRLA->SWRST)=1;
#define USB_ENABLE (p_USB_CTRLA->ENABLE)=1;
#define USB_DISABLE (p_USB_CTRLA->DISABLE)=1;

#define USB_MODE_DEVICE_SET (p_USB_CTRLA->MODE)=0;
#define USB_MODE_HOST_SET (p_USB_CTRLA->MODE)=1;

/* Synchronization Busy */
typedef struct {
  const volatile uint8_t SWRST:1;
  const volatile uint8_t ENABLE:1;
} USB_SYNCBUSY_t;

#define USB_SYNCBUSY_OFFSET 0x02

#define p_USB_SYNCBUSY ((USB_SYNCBUSY_t * const)(USB_ADDRESS + USB_SYNCBUSY_OFFSET))

#define USB_SYNCBUSY_ENABLE_SPIN {  \
  while( 1 == p_USB_SYNCBUSY->ENABLE ) {} \
}


/* Pad Calibration */
typedef struct {
  volatile uint16_t TRANSP:5;
  volatile uint16_t TRANSN:5;
  volatile uint16_t :1;
  volatile uint16_t TRIM:3;
  volatile uint16_t :1;
} USB_PADCAL_t;

#define USB_PADCAL_OFFSET 0x28

#define p_USB_PADCAL ((USB_PADCAL_t * const)(USB_ADDRESS + USB_PADCAL_OFFSET))

#define USB_CAL_SET(calval,value) p_USB_PADCAL->calval=value;

#endif /* __USB_H__ */
