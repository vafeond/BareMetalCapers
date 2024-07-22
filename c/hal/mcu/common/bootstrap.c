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

/* references:
 * https://sourceware.org/binutils/docs/ld/Source-Code-Reference.html
 */

/******************************************************************************
 * External Symbols
 */

/* from linker command script: bootstrap copy area */
extern char __ld_copy_src_start[];
extern char __ld_copy_dst_start[];
extern char __ld_copy_dst_end[];

/* from linker command script: bootstrap clear area */
extern char __ld_clear_start[];
extern char __ld_clear_end[];

/******************************************************************************
 * Bootstrap
 * perform initialization that user code should assume is completed
 */
static void hal_bootstrap_copy ( void )
{
  char *p_src = __ld_copy_src_start;
  char *p_dst = __ld_copy_dst_start;

  while ( p_dst < __ld_copy_dst_end )
  {
    *p_dst++ = *p_src++;
  }
}

static void hal_bootstrap_clear ( void )
{
  char* p_dst = __ld_clear_start;
  while ( p_dst < __ld_clear_end )
  {
    *p_dst++ = 0;
  }
}

void hal_bootstrap ( void )
{
  hal_bootstrap_copy();
  hal_bootstrap_clear();
}

