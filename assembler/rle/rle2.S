#include "printx.hs"
.section .data.MyData
   V: .long 0x0
   .global A
   A1: .long 0x1, 0x0, 0x0, 0x11, 0x12, 0x13, 0x6, 0x6, 0x6, 0x6 
   A2: .long 0x0, 0x0, 0x0, 0x11, 0x12, 0x13, 0x6, 0x7, 0x8, 0x9 
   .global B
   B: .long 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
   C: .long 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
.section .text.AAA

.global __main

__main:
ar0 = A1;
ar1 = B;
ar2 = 10;

gr0 = 1; // i
gr1 = 0; // j
gr3 = 1; // cnt1
gr4 = 0; // cnt2
gr5 = 0; // begin
for:
    gr6 = ar2;
    gr6-gr0;
    if <= goto endfor;
        gr6 = [ar0+gr0];
        gr0--;
        gr7 = [ar0+gr0];
        gr0++;
        gr6 - gr7;
        if =0 goto iter;
        gr6 = 1;
        gr3 - gr6;
        if <>0 goto endif1;
            gr4++;
            gr3 = 0;
            goto iter;
        endif1:
        gr4;
        if =0 goto endif2;
            
            gr6 = - gr4;
            [ar1++] = gr6;
            gr7 = gr0;
            gr0 = gr5;
            ///векторное копирование?
            for1:
                gr6 = [ar0+gr0];
                gr0++;
                [ar1++] = gr6;

                gr4--;
                if > goto for1;
            gr0 = gr7;
        endif2:

        gr5 = gr0;
        [ar1++] = gr3;
        gr7 = gr0;
        gr0 = gr1; 
            gr6 = [ar0+gr0];
            [ar1++] = gr6;
        gr0 = gr7;
        gr3 = 0;
    iter: 
        gr0++;
        gr1++;
        gr3++;
    goto for;
endfor:

gr6 = 1;
gr3 - gr6;
if <>0 goto endif3;
    gr4++;
    gr6 = -gr4;
    [ar1++] = gr6;
    gr0 = gr5;
    ///векторное копирование?
    for4:
        gr6 = [ar0+gr0];
        gr0++;
        [ar1++] = gr6;

        gr4--;
        if > goto for4;
    goto end;
endif3:

gr4;
if =0 goto endif4;
    gr6 = -gr4;
    [ar1++] = gr6;
    gr0 = gr5;
    ///векторное копирование?
    for5:
        gr6 = [ar0+gr0];
        gr0++;
        [ar1++] = gr6;

        gr4--;
        if > goto for5;
endif4:

[ar1++] = gr3;
gr0 = gr1; 
gr6 = [ar0+gr0];
[ar1++] = gr6;
end:
PRINTF_VEC(B, 15)
end1:

    ar0 = B;
    gr3 = ar1;
    gr4 = B;
    gr3 -= gr4;
    ar1 = C;

    gr0 = 0;
    for_:
        gr3 - gr0;
        if <= goto endfor_;
        gr4 = [ar0+gr0];
        gr0++;
        gr4;
        if < goto endif1_;
            gr5 = [ar0+gr0];
            gr0++;
            for1_:
                [ar1++] = gr5; 
                gr4--;
                if > goto for1_;
            goto for_;
        endif1_:
        for2_:
            gr5 = [ar0+gr0];
            [ar1++] = gr5;
            gr0++;
            gr4++;
            if < goto for2_;
        goto for_;
    endfor_:
PRINTF_VEC(C, 15)

gr7 = 0;
return;



