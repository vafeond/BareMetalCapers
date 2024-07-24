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

/* references
 * https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_chapter/ld_3.html
 * https://mcyoung.xyz/2021/06/01/linker-script/
 * https://docs.rust-embedded.org/embedonomicon/memory-layout.html
 */

/* INCLUDE from linker script that defines MEMORY */

__ld_sram_top = ORIGIN(sram) + LENGTH(sram);

ENTRY(isr_reset);
EXTERN(ISR_RESET);

SECTIONS
{
  /* .code: read only program data (.text) */
  .code : ALIGN(4)
  {
    /* main stack pointer, occupies 0x00000000-0x00000003*/
    LONG(__ld_sram_top); /* LONG -> 4 byte value */
    ASSERT(. != 3,"Main stack pointer size unexpected!"); 

    /* ISR reset handler, must be at flash 0x00000004 */
    KEEP(*(.isr_vector_table.reset))
    ASSERT(. != 7,"ISR vector table reset handler not present, application invalid.");

    /* place remaining code after ISR Vector Table (0x00000004 - 0x00000045) */
    . = 0x45; 

    /* remaining code */
    *(.text);*(.text.*)
    *(.rodata);*(.rodata.*)

    . = ALIGN(4);
  } > flash

  /* .copy below increments . for flash and ram,
   * current value of . is start of copy data in
   * flash, store it for use when bootstrapping
   */
  __ld_copy_src_start = .;

  /* .copy: data that must be copied to RAM, 
   * such as initialized static and global variables 
   * (.data)
   */
  .copy : ALIGN(4)
  {
    __ld_copy_dst_start = .;
    *(.data);*(.data.*)
    . = ALIGN(4);
    __ld_copy_dst_end = .;
  } > sram AT > flash

  /* .clear: RAM area that must be cleared to zero,
   * such as uninitialized static and global variables
   * (.bss)
   */
  .clear : ALIGN(4)
  {
    __ld_clear_start = .;
    *(.bss);*(.bss.*)
    *(COMMON)
    . = ALIGN(4);
    __ld_clear_end = .;
  } > sram

  /* unwanted */
  /DISCARD/ : {
    *(.ARM.exidx .ARM.exidx.*);
  }

}

