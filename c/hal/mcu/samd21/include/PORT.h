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

#if !defined __PORT_H__
#define __PORT_H__

#include <stdint.h>
#include "product_mapping.h"


/******************************************************************************
 * Memory Mappings
 */
#define PORT_ADDRESS (AHB_APB_BRIDGE_B_ADDRESS + PORT_OFFSET)

#define GROUP_A_OFFSET 0x00
#define GROUP_B_OFFSET 0x80
#define GROUP_C_OFFSET 0x100


/******************************************************************************
 * PORT (sans PMUX and PINCFG)
 */
typedef struct {
  volatile uint32_t PINMASK:16;
  volatile uint32_t PMUXEN:1;
  volatile uint32_t INEN:1;
  volatile uint32_t PULLEN:1;
  volatile uint32_t :3;
  volatile uint32_t DRVSTR:1;
  volatile uint32_t :1;
  volatile uint32_t PMUX:4;
  volatile uint32_t WRPMUX:1;
  volatile uint32_t :1;
  volatile uint32_t WRPINCFG:1;
  volatile uint32_t HWSEL:1;
} WRCONFIG_t;

typedef struct {
  volatile uint32_t DIR;
  volatile uint32_t DIRCLR;
  volatile uint32_t DIRSET; 
  volatile uint32_t DIRTGL; 

  volatile uint32_t OUT;
  volatile uint32_t OUTCLR;
  volatile uint32_t OUTSET; 
  volatile uint32_t OUTTGL; 

  volatile uint32_t IN;

  volatile uint32_t CTRL;
  WRCONFIG_t        WRCONFIG;
} PORT_t;

#define p_PORT_GROUP(group) ((PORT_t * const)(PORT_ADDRESS + GROUP_ ## group ## _OFFSET))

#define PIN_SET_MASK(pinId) (0x1<<pinId)
#define PIN_CLR_MASK(pinId) (~(0x1<<pinId))

#define PORT_PIN_DIR_OUT(group,pinId) p_PORT_GROUP(group)->DIRSET=PIN_SET_MASK(pinId);
#define PORT_PIN_DIR_IN(group,pinId) p_PORT_GROUP(group)->DIRCLR=PIN_SET_MASK(pinId);
#define PORT_PIN_DIR_TGL(group,pinId) p_PORT_GROUP(group)->DIRTGL=PIN_SET_MASK(pinId);

#define PORT_PIN_OUT_SET(group,pinId) p_PORT_GROUP(group)->OUTSET=PIN_SET_MASK(pinId);
#define PORT_PIN_OUT_CLR(group,pinId) p_PORT_GROUP(group)->OUTCLR=PIN_SET_MASK(pinId);
#define PORT_PIN_OUT_TGL(group,pinId) p_PORT_GROUP(group)->OUTTGL=PIN_SET_MASK(pinId);

#define PORT_PIN_READ(group,pinId) ((p_PORT_GROUP(group)->IN&PIN_SET_MASK(pinId))>>(pinId-1))

/******************************************************************************
 * PORT PMUX
 */
typedef struct {
  volatile uint8_t PMUXE:4; /* even numbered pins */
  volatile uint8_t PMUXO:4; /* odd numbered pins */
} PORT_PMUX_t;
enum PORT_PMUX_Peripheral {
  PERIPHERAL_A = 0x00,
  PERIPHERAL_B = 0x01,
  PERIPHERAL_C = 0x02,
  PERIPHERAL_D = 0x03,
  PERIPHERAL_E = 0x04,
  PERIPHERAL_F = 0x05,

  PERIPHERAL_G = 0x06,
  PERIPHERAL_USB = PERIPHERAL_G,

  PERIPHERAL_H = 0x07,
  PERIPHERAL_I = 0x08
};

#define PMUX0_OFFSET 0x30
#define p_PORT_PMUX_GROUP(group) ((PORT_PMUX_t * const)(PORT_ADDRESS + GROUP_ ## group ## _OFFSET + PMUX0_OFFSET))
#define p_PORT_PMUX(group,pinId) ((PORT_PMUX_t* const)(p_PORT_PMUX_GROUP(group)+((pinId/2)*sizeof(PORT_PMUX_t)))
#define p_PORT_PMUX_SET(group,pinId,peripheral) {                 \
  if (pinId&1) p_PORT_PMUX(group,pinId)->PMUXO=peripheral;        \
  else p_PORT_PMUX(group,pinId)->PMUXE=peripheral;                \
}

/******************************************************************************
 * PORT PINCFG
 */
typedef struct {
  volatile uint8_t PMUXEN:1;
  volatile uint8_t INEN:1;
  volatile uint8_t PULLEN:1;
  volatile uint8_t :3;
  volatile uint8_t DRVSTR:1;
  volatile uint8_t :1;
} PORT_PINCFG_t;

#define PINCFG0_OFFSET 0x40
#define p_PORT_PINCFG_GROUP(group) ((PORT_PINCFG_t * const)(PORT_ADDRESS + GROUP_ ## group ## _OFFSET + PINCFG0_OFFSET))
#define p_PORT_PINCFG(group,pinId) ((PORT_PINCFG_t* const)(p_PORT_PINCFG_GROUP(group)+(pinId*sizeof(PORT_PINCFG_t))))


/******************************************************************************
 * Peripheral Helpers
 */

// enabling SOF (Start of Frame) is optional
#define PORT_PERIPHERAL_USB_ENABLE(b_SOF) {       \
  if(b_SOF)p_PORT_PMUX_SET(A,23,PERIPHERAL_USB);  \
  p_PORT_PMUX_SET(A,24,PERIPHERAL_USB);           \
  p_PORT_PMUX_SET(A,25,PERIPHERAL_USB);           \
  if(b_SOF)p_PORT_PINCFG(A,23)->PMUXEN=1;         \
  p_PORT_PINCFG(A,24)->PMUXEN=1;                  \
  p_PORT_PINCFG(A,25)->PMUXEN=1;                  \
}

// If the PA24 and PA25 pins are not connected, it is recommended to enable a pull-up on PA24 and PA25
// through input GPIO mode. The aim is to avoid an eventually extract power consumption (<1mA) due to a not
// stable level on pad. The port PA24 and PA25 doesn't have Drive Strength option.
#define PORT_PERIPHERAL_USB_DISABLE {                                 \
  p_PORT_PINCFG(A,23)->PMUXEN=0;                                      \
  p_PORT_PINCFG(A,24)->PMUXEN=0;                                      \
  p_PORT_PINCFG(A,25)->PMUXEN=0;                                      \
  /* enable pull up for these pins as recommended by the datasheet */ \
  p_PORT_PINCFG(A,24)->PULLEN=1;                                      \
  p_PORT_PINCFG(A,25)->PULLEN=1;                                      \
}

#endif /* __PORT_H__ */
