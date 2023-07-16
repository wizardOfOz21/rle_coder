#include "printx.hs"

.global __main

.section .data.MyData
   .global A
   A: .long 0x0, 0x0, 0x0, 0x11, 0x11, 0x11, 0x6, 0x7, 0x8, 0x8, 0x8
   .global B
   B: .long 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0

.section .text.AAA
__main:
  ar0 = A;
  ar1 = B;
  gr2 = [ar0++];
  gr0 = 10;
Loop1:
    gr1 = gr2;
    gr4 = 1;
    Loop2:
      gr2 = [ar0++];
      gr3 = gr2 - gr1;
      if <>0 goto Loop1_c;
      gr4++;
      gr0--;
      if > goto Loop2;
    Loop1_c:
    gr0--;
    [ar1++] = gr4;
    [ar1++] = gr1;
    if > goto Loop1;

  ar1 = B;
  gr0 = 11;
  Loop3:
    gr1 = [ar1++];
    PRINTF1("%x ",gr1)
    gr0--;
    if > goto Loop3;


  gr7 = 0;
  return;	

