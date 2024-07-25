
## Memory Map Modelling Rationale
The microcontroller's memory map must be defined in code. There are
multiple ways to do this. Ideally, the chosen method is developer
friendly, but embedded systems need to be cognizant of resources
too; therefore experimenting with the compiler and linker can be
illuminating.

*The below example also demonstrates uses of `.code`, `.copy`, and
`.clear` sections as managed in the linker script.*

Consider modelling the SAMD21 default clock, OSC8M. We have:
* OSC8M offset: `0x20`
* SYSCTRL offset: `0x800`
* AHB-APB Bridge address: `0x40000000`

The OSC8M control register address is `0x40000820`, which can
be modelled as,
```
#define AHB_APB_BRIDGE_A_ADDRESS 0x40000000
#define SYSCTRL_OFFSET 0x800
#define OSC8M_OFFSET 0x20
#define OSC8M_ADDRESS (AHB_APB_BRIDGE_A_ADDRESS + SYSCTRL_OFFSET + OSC8M_OFFSET)
```

The control register is 32 bits wide and consists of several internal 
bit-fields, which is nicely supported by 
[C bit-fields](https://en.cppreference.com/w/c/language/bit_field).
```
typedef struct {
  uint32_t :1;                    /* 0: unused */
  uint32_t ENABLE :1;             /* default: 1 */
  uint32_t :4;                    /* 2-5: unused */
  uint32_t RUNSTDBY:1;            /* default: 0 */
  uint32_t ONDEMAND:1;            /* default: 1*/
  uint32_t PRESC:2;               /* default: 0x3 (divide by eight) */
  uint32_t :6;                    /* 10-15: unused */
  const uint32_t CALIB_TEMP:6;    /* written during manufacture, protecting from overwrite */
  const uint32_t CALIB_OVERALL:6; /* written during manufacture, protecting from overwrite */
  uint32_t :2;                    /* 28-29: unused */
  const uint32_t FRANGE:2;        /* written during manufacture, protecting from overwrite */
} OSC8M_t;
```

To check the behavior of the compiler and linker:
* Include desired compiler optimizations using the build system `release` target
* Check the generated ELF file using the build system `dump` target

### Approach 1 - Global OSC8M pointer
Create a global pointer that is initialized to the OSC8M control register
address. This is a reasonable developer experience as a structure that requires 
dereferencing is intuitive for C developers.

```
OSC8M_t * p_OSC8M = ((OSC8M_t*)(AHB_APB_BRIDGE_A_ADDRESS + SYSCTRL_OFFSET + OSC8M_OFFSET));
p_OSC8M->PRESC = 0; /* use 8Mhz clock */
```

*Sections*
```
Idx Name          Size      VMA       LMA       File off  Algn
  0 .code         00000090  00000000  00000000  00001000  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .copy         00000004  20000000  00000090  00002000  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  2 .clear        00000000  20000004  00000094  00002004  2**2
                  ALLOC
```

*Symbols (abridged)*
```
20000000 g     O .copy  00000004 p_OSC8M
```

`p_OSC8M` is a non-const, initialized, global variable; meaning it has an
initial value that must be stored in flash (`.code` section), which must
be copied into RAM (`.copy` section).

So the `.copy` section has a size of four.
The symbol table also includes an entry.

### Approach 2 - Global constant OSC8M pointer
Same as above but tell the comiler that the value of the pointer cannot
change.

```
OSC8M_t * const p_OSC8M = ((OSC8M_t*)(AHB_APB_BRIDGE_A_ADDRESS + SYSCTRL_OFFSET + OSC8M_OFFSET));
p_OSC8M->PRESC = 0; /* use 8Mhz clock */
```

*Sections*
```
Idx Name          Size      VMA       LMA       File off  Algn
  0 .code         00000094  00000000  00000000  00001000  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .copy         00000000  20000000  00000094  00002000  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  2 .clear        00000000  20000000  00000094  00002000  2**2
                  ALLOC
```

*Symbols (abridged)*
```
00000090 g     O .code  00000004 p_OSC8M
```

A constant variable doesn't need to reside in RAM, so `.copy` size is
zero. The symbol is captured in flash, so the `.code` section is
four bytes larger than approach 1.

### Approach 3 - OSC8M pointer macro
Specifying the address as a macro means the preprocessor will replace
the macro with the literal value of the address in the code. This is my
least favourite developer experience. Treating a macro like a pointer to
a structure is less natural, making the interface slightly more difficult 
to use.

```
#define p_OSC8M ((OSC8M_t * const)(AHB_APB_BRIDGE_A_ADDRESS + SYSCTRL_OFFSET + OSC8M_OFFSET))
p_OSC8M->PRESC = 0; /* use 8MHz clock */
```

*Sections*
```
Idx Name          Size      VMA       LMA       File off  Algn
  0 .code         00000090  00000000  00000000  00001000  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .copy         00000000  20000000  00000090  00002000  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  2 .clear        00000000  20000000  00000090  00002000  2**2
                  ALLOC
```

This is a clear winner when evaluating the resources used.
There isn't a symbol, meaning it doesn't have to be accounted for in `.code` 
or `.copy` sections.

### Conclusion
I expected the compiler to optimize away a constant global and deliver the
same results as the macro approach. If so, the nicer developer experience 
could be supported without a performance penatly, but that is not the case.

In order to keep the `.code` and `.copy` sections minimal, and avoid littering 
the executable with symbols for microcontroller peripherals not used by the
application, the macro approach will be taken.

Note: Optimization flag `-0s` was used above, but the same behavior is 
observed with `-O3`.

