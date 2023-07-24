#include "printx.hs"

.global __main

.section .data.MyData
   V: .long 0x0
   .global A
   A: .long 0x0, 0x0, 0x0, 0x11, 0x11, 0x11, 0x6, 0x7, 0x6, 0x6, 0x6
   .global B
   B: .long 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0

.section .text.AAA
__main:
  ar0 = A;
  ar1 = B;
  gr2 = [ar0++];
  gr0 = 11;
  gr0--;
Loop1:
    gr1 = gr2;
    gr4 = 1;
    Loop2:
      gr2 = [ar0++];
      gr2 - gr1;
      if <>0 goto Loop1_c;
      gr0--;
      if > delayed goto Loop2;
      gr4++;
      nul;
    Loop1_c:
    gr0--;
    [ar1++] = gr4;
    [ar1++] = gr1;
    if > goto Loop1;

  gr2-gr1;
  if =0 goto End;
    gr0 = 1;
    [ar1++] = gr0;
    [ar1++] = gr2;
  End:

  PRINTF_VEC(B, 15)
  gr7 = 0;
  return;	

