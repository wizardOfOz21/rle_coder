#include "printx.hs"

.global __main

.section .data.MyData
   .global A
   A: .float 1.25, 33554433, 33554434, 33554435, 33554436, 16777222, 16777223, 16777224
   STR: .string "bee"
   .global B
   B:  .quad 0x0706050403020100, 0x1716151413121110, 0x2726252423222120, 0x3736353433323130
   matr00: .long 0x87654321//, 7FFFC101hl, 000000000000FFFFhl, 00000000FFFF0000hl, 0000FFFF00000000hl, FFFF000000000000hl, 0000FFFFFFFF0000hl, 9C3E1FFFC1FF83FFhl;
.section .bss.MyData1
  C:  .space 1<<6

.section .text.AAA
__main:
  ar0 = A;
  fpu 0 rep 8 vreg0 = [ar0++];
  fpu 0 .float vreg0 = vreg0 * vreg0;
  ar0 = A;
  fpu 0 rep 4 [ar0++] = vreg0;
  PRINTF_VEC(A, 8)

  gr7 = 0;
  return;	

