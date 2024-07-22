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

#if !defined __PRODUCT_MAPPING_H__
#define __PRODUCT_MAPPING_H__

/* CODE */
/* Flash and SRAM provided as linker symbols */
#define INTERNAL_RWWEE_ADDRESS 0x00000000
#define INTERNAL_RWWEE_SIZE 0

/* Global Memory Space */
#define PERIPHERALS_ADDRESS 0x40000000
#define IOBUS_ADDRESS 0x60000000
#define SYSTEM_ADDRESS 0xE0000000

/* AHB-APB Bridge A */
#define AHB_APB_BRIDGE_A_ADDRESS 0x40000000
#define PAC0_OFFSET 0x00
#define PM_OFFSET 0x400
#define SYSCTRL_OFFSET 0x800
#define GCLK_OFFSET 0xC00
#define WDT_OFFSET 0x1000
#define RTC_OFFSET 0x1400
#define EIC_OFFSET 0x1800

/* AHB-APB Bridge B */
#define AHB_APB_BRIDGE_B_ADDRESS 0x41000000
#define PAC1_OFFSET 0
#define DSU_OFFSET 0x2000
#define NVMCTRL_OFFSET 0x4000
#define PORT_OFFSET 0x4400
#define DMAC_OFFSET 0x4800
#define USB_OFFSET 0x5000
#define MTB_OFFSET 0x6000

/* AHB-APB Bridge C */
#define AHB_APB_BRIDGE_C_ADDRESS 0x42000000
#define PAC2_OFFSET 0
#define EVSYS_OFFSET 0x400
#define SERCOM0_OFFSET 0x800
#define SERCOM1_OFFSET 0xC00
#define SERCOM2_OFFSET 0x1000
#define SERCOM3_OFFSET 0x1400
#define SERCOM4_OFFSET 0x1800
#define SERCOM5_OFFSET 0x1C00
#define TCC0_OFFSET 0x2000
#define TCC1_OFFSET 0x2400
#define TCC2_OFFSET 0x2800
#define TC3_OFFSET 0x2C00
#define TC4_OFFSET 0x3000
#define TC5_OFFSET 0x3400
#define TC6_OFFSET 0x3800
#define TC7_OFFSET 0x3C00
#define ADC_OFFSET 0x4000
#define AC_OFFSET 0x4400
#define DAC_OFFSET 0x4800
#define PTC_OFFSET 0x4C00
#define I2S_OFFSET 0x5000
#define AC1_OFFSET 0x5400
#define TCC3_OFFSET 0x6000

#endif /* __PRODUCT_MAPPING_H__ */
