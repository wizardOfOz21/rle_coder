#include "printx.hs"

.global _Encode // объявление метки с именем подпрограммы.
.text
_Encode:
  ar5 = ar7 - 2; // адреса в стеке для доступа к входным параметрам.
  push ar0, gr0; // сохранение используемых в подпрограмме регистров.
  push ar1, gr1;
  push ar2, gr2;
  push ar3, gr3;
  ar0 = [--ar5];
  PRINTF1("=%x\n",ar0)
  ar1 = [--ar5];
  PRINTF1("=%x\n",ar1)
  gr0 = [--ar5];
  PRINTF1("=%x\n",gr0)

  gr2 = [ar0++] with gr0--;
Loop1:
    gr1 = gr2;
    gr3 = 1;
    Loop2:
      gr2 = [ar0++];
      gr2 - gr1;
      if <>0 goto Loop1_c;
      gr3++;
      gr0--;
      if > goto Loop2;
    Loop1_c:
    gr0--;
    [ar1++] = gr3;
    [ar1++] = gr1;
    if > goto Loop1;

  gr2-gr1;
  if =0 goto End;
    gr0 = 1;
    [ar1++] = gr0;
    [ar1++] = gr2;
  End:

  gr7 = 0;
  pop ar3, gr3;
  pop ar2, gr2;
  pop ar1, gr1;
  pop ar0, gr0; // восстановление значений регистров при выходе
  return;
