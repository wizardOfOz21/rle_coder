#define FPU_COPY(Ind) FPU_COPY_MACRO Ind

.macro FPU_COPY_MACRO ind
        vector_write\ind:
        gr4 = ar4;
        gr7 = ar1;
        gr5 = gr4 xor gr7;
        gr7 = 1;
        gr5 and gr7;
        if <>0 goto imp\ind with gr4 and gr7;
        if =0 goto even\ind;
            gr4 = [ar4++];
            [ar1++] = gr4;
            gr2--;
        even\ind:
        gr5 = 64;
        check_size\ind:
            gr2 - gr5;
            if >= delayed goto write\ind;
                gr7 = gr2 >> 1;
            if =0 delayed goto imp\ind with gr7--;
                vlen = gr7;
                nul;

            fpu 0 rep vlen vreg0 = [ar4++];
            fpu 0 rep vlen [ar1++] = vreg0;

            gr7 = 1;
            delayed goto check_size\ind;
                gr2 = gr2 and gr7;
        write\ind:
            // rep 32 data = [ar4++] with data;
            // rep 32 [ar1++] = afifo;
            fpu 0 rep 32 vreg0 = [ar4++];
            fpu 0 rep 32 [ar1++] = vreg0;
            delayed goto check_size\ind;
                gr2 -= gr5;
        imp\ind:

        gr2;
        if =0 goto end\ind;

        gr2--;
        for\ind\ind:
        if > delayed goto for\ind\ind with gr2--;
            gr4 = [ar4++];
            [ar1++] = gr4;

        end\ind:
.endm
