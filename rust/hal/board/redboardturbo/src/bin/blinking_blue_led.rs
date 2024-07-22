//******************************************************************************
//  This is free and unencumbered software released into the public domain.
//
//  Anyone is free to copy, modify, publish, use, compile, sell, or
//  distribute this software, either in source code form or as a compiled
//  binary, for any purpose, commercial or non-commercial, and by any
//  means.
//
//  In jurisdictions that recognize copyright laws, the author or authors
//  of this software dedicate any and all copyright interest in the
//  software to the public domain. We make this dedication for the benefit
//  of the public at large and to the detriment of our heirs and
//  successors. We intend this dedication to be an overt act of
//  relinquishment in perpetuity of all present and future rights to this
//  software under copyright law.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.
//
//  For more information, please refer to <https://unlicense.org>

#![no_std]
#![no_main]

extern crate samd21;

/******************************************************************************
 * RedBoard Turbo
 * Blue LED: PA17
 */

/* Product Mapping: AHB-APB bridges */ 
// const AHB_APB_BRIDGE_A:u32 = 0x40000000;
const AHB_APB_BRIDGE_B:u32 = 0x41000000;
// const AHB_APB_BRIDGE_C:u32 = 0x42000000;

/* Product Mapping: PORT offset */ 
const PORT:u32 = 0x4400;

/* PORT configuration and control offsets */
// const DIR:u32 = 0x00;
// const DIRCLR:u32 = 0x04 ;
const DIRSET:u32 = 0x08 ;
// const DIRTGL:u32 = 0x0C; /* toggle direction */

// const OUT:u32 = 0x10 ;
const OUTCLR:u32 = 0x14 ;
// const OUTSET:u32 = 0x18;
const OUTTGL:u32 = 0x1C; /* toggle output (hi/low) */

/* PA helper macros */
const PA_CONFIG_OUTPUT: *mut u32 = (AHB_APB_BRIDGE_B + PORT + DIRSET) as *mut u32;
// const PA_CONFIG_INPUT: *mut u32 = (AHB_APB_BRIDGE_B + PORT + DIRCLR) as *mut u32;

const PA_CLR_OUTPUT: *mut u32 = (AHB_APB_BRIDGE_B + PORT + OUTCLR) as *mut u32;
const PA_TOGGLE_OUTPUT: *mut u32 = (AHB_APB_BRIDGE_B + PORT + OUTTGL) as *mut u32;

/* set bit 17 for blue LED */
const PA17_SET_MASK:u32 = 0x1<<17;


/******************************************************************************
 * User Code
 */ 
#[no_mangle]
pub fn main() -> ! {
    let mut count:u32 = 0;

    unsafe {
        *PA_CONFIG_OUTPUT = PA17_SET_MASK;
        *PA_CLR_OUTPUT = PA17_SET_MASK;
    }

    // let toggle_delay_cnt = 50000;
    let toggle_delay_cnt:u32 = 20000;

    loop {
        count+=1; 
        if count >= toggle_delay_cnt {
            unsafe {
                *PA_TOGGLE_OUTPUT = PA17_SET_MASK;
            }
          count=0;
        }
    }
}
