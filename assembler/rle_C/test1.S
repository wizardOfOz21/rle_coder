#include "printx.hs"

.global _Encode // объявление метки с именем подпрограммы.
.text
_Encode:
    ar5 = ar7 - 2; // адреса в стеке для доступа к входным параметрам.
    push ar0, gr0; // сохранение используемых в подпрограмме регистров.
    push ar1, gr1;
    push ar2, gr2;
    push ar3, gr3;
    push ar4, gr4;
    ar0 = [--ar5];
    ar1 = [--ar5];
    gr4 = [--ar5];

    gr3 = ar0;
    gr3 += gr4; //first
    ar4 = ar0;  // begin
    ar0++;      // i
    
    ar2 = ar1;
    gr1 = 1; // cnt1
    gr2 = 0; // cnt2

    gr4 = ar0;
    gr3 - gr4;
    for:
        if <= goto endfor;
            gr4 = [ar0];
            ar0--;
            gr7 = [ar0++];
            gr4 - gr7;
            if =0 goto iter;
            gr4 = 1;
            gr1 - gr4;
            if <>0 goto endif1 with gr2;
                gr2++;
                gr1 = 0;
                goto iter;
            endif1:

            if =0 goto endif2;

                gr4 = - gr2;
                [ar1++] = gr4;
                ///векторное копирование?
                gr2--;
                for1:
                    if > delayed goto for1 with gr2--;
                        gr4 = [ar4++];
                        [ar1++] = gr4;
                gr2 = 0;
            endif2:

            ar4 = ar0;
            [ar1++] = gr1;
            gr4 = [--ar0];
            [ar1++] = gr4;
            ar0++;
            gr1 = 0;
        iter: 
            ar0++;
            gr1++;
        delayed goto for;
            gr4 = ar0;
            gr3 - gr4;
    endfor:

    gr4 = 1;
    gr1 - gr4;
    if <>0 goto endif3 with gr2;
        gr2++;
        gr4 = -gr2;
        [ar1++] = gr4;
        ///векторное копирование?
        gr2--;
        for4:
            if > delayed goto for4 with gr2--;
                gr4 = [ar4++];
                [ar1++] = gr4;
        goto end;
    endif3:

    if =0 goto endif4;
        gr4 = -gr2;
        [ar1++] = gr4;
        ///векторное копирование?
        gr2--;
        for5:
            if > delayed goto for5 with gr2--;
                gr4 = [ar4++];
                [ar1++] = gr4;
    endif4:

    [ar1++] = gr1;
    gr4 = [--ar0];
    [ar1++] = gr4;
    end:
    gr7 = ar1;
    gr4 = ar2;
    gr7 -= gr4;
    pop ar4, gr4;
    pop ar3, gr3;
    pop ar2, gr2;
    pop ar1, gr1;
    pop ar0, gr0; // восстановление значений регистров при выходе
    return;


.global _Decode // объявление метки с именем подпрограммы.
.text
_Decode:
    ar5 = ar7 - 2; // адреса в стеке для доступа к входным параметрам.
    push ar0, gr0; // сохранение используемых в подпрограмме регистров.
    push ar1, gr1;
    push ar2, gr2;
    ar0 = [--ar5];
    ar1 = [--ar5];
    gr2 = [--ar5];

    gr0 = ar0;
    gr0 += gr2;

    gr2 = ar0;
    gr0 - gr2;
    for_:
        if <= delayed goto endfor_;
            gr2 = [ar0++];
            gr2;
        if < goto endif1_;
            gr1 = [ar0++];
            gr2--;
            for1_:
                if > delayed goto for1_ with gr2--;
                    [ar1++] = gr1;
                    nul; 
            delayed goto for_;
                gr2 = ar0;
                gr0 - gr2;
        endif1_:

        gr2++;
        for2_:
            if < delayed goto for2_ with gr2++;
                gr1 = [ar0++];
                [ar1++] = gr1;

            delayed goto for_;
                gr2 = ar0;
                gr0 - gr2;
    endfor_:

    pop ar2, gr2;
    pop ar1, gr1;
    pop ar0, gr0; // восстановление значений регистров при выходе
    gr7 = 0;
    return;
