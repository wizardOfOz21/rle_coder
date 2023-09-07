#include "printx.hs"
#include "macro.h"

.global _Encode1
.text
_Encode1:
    ar5 = ar7 - 2;
    push ar0, gr0;
    push ar1, gr1;
    push ar2, gr2;
    push ar3, gr3;
    push ar4, gr4;
    push ar5, gr5;
    ar0 = [--ar5];
    ar1 = [--ar5];
    gr4 = [--ar5];

    ar2 = ar1;

    gr3 = ar0;
    gr3 += gr4;
    gr0 = gr3;
    gr3--;
    gr1 = 0;   //cnt1
    gr2 = 0;   //cnt2
    ar4 = ar0; // begin

    gr4 = [ar0];
    gr7 = [ar0+1];
    gr4 - gr7;
    if =0 goto endif1;
        gr4 = ar0;
        gr3 - gr4;
        while1:
        if <= delayed goto end_while_with_eof1;
                gr4 = [ar0];
                gr7 = [ar0+1];

            gr4 - gr7;
            if =0 goto end_while1;
                ar0++ with gr2++;
                delayed goto while1;
                    gr4 = ar0;
                    gr3 - gr4;

        end_while_with_eof1:
            gr2++;
            gr4 = - gr2;
            [ar1++] = gr4;
            ///векторное копирование?
            // gr2--;
            // for1:
            // if > delayed goto for1 with gr2--;
            //     gr4 = [ar4++];
            //     [ar1++] = gr4;
            FPU_COPY(1)
            goto end;
        end_while1:

        gr4 = - gr2;
        [ar1++] = gr4 /*with gr2--*/;
        ///векторное копирование?
        // for2:
        // if > delayed goto for2 with gr2--;
        //     gr4 = [ar4++];
        //     [ar1++] = gr4;
        FPU_COPY(2)
    endif1:

    gr7 = [ar0++] with gr2 = gr2 - gr2;
    gr4 = ar0;
    gr0 - gr4;
    while_true:
        do_while1:
            if > delayed goto skip_return;
                    gr4 = [ar0];
                    gr4 - gr7;
                gr1++;
                [ar1++] = gr1;
                [ar1++] = gr7;
                goto end;
            skip_return:
            ar0++;
            if =0 delayed goto do_while1 with gr1++;
                gr4 = ar0;
                gr0 - gr4;

        ar0--;
        [ar1++] = gr1;
        [ar1++] = gr7 with gr1 = gr1 - gr1;

        gr4 = ar0;
        ar4 = ar0 with gr3 - gr4;
        while2:
            if <= delayed goto end_while_with_eof2;
                    gr4 = [ar0];
                gr7 = [ar0+1];
                gr4 - gr7;
                if =0 goto end_while2 with gr2;
                    ar0++ with gr2++;
                    delayed goto while2;
                        gr4 = ar0;
                        gr3 - gr4;

        end_while_with_eof2:
            gr2++;
            gr4 = - gr2;
            [ar1++] = gr4 /*with gr2--*/;
            // for3:
            // if > delayed goto for3 with gr2--;
            //     gr4 = [ar4++];
            //     [ar1++] = gr4;
            FPU_COPY(3)
            goto end;
        end_while2:

        if =0 delayed goto while_true_;
            gr4 = - gr2;
            nul;
        [ar1++] = gr4;

        FPU_COPY(4)
        
        // gr2;
        // if =0 goto while_true_;

        // gr2--;
        // for4:
        // if > delayed goto for4 with gr2--;
        //     gr4 = [ar4++];
        //     [ar1++] = gr4;

    while_true_:
        gr7 = [ar0++];
    delayed goto while_true with gr2 = gr2 - gr2;
        gr4 = ar0;
        gr0 - gr4;

    end:
    gr7 = ar1;
    gr4 = ar2;
    gr7 -= gr4;
    pop ar5, gr5;
    pop ar4, gr4;
    pop ar3, gr3;
    pop ar2, gr2;
    pop ar1, gr1;
    pop ar0, gr0;
    return;
