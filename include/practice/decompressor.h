#ifndef DECOMPESSOR_DOT_H
#define DECOMPESSOR_DOT_H

#ifdef LOG
#include <iostream>
#include <iomanip>
#endif

#include "byteio.h"
#include "bitio.h"

template<typename INPUT, typename OUTPUT, typename MODEL>
class decompressor
{
  typedef typename MODEL::CODE_VALUE CODE_VALUE;
  typedef typename MODEL::prob prob;
public :
  decompressor(INPUT &input, OUTPUT &output, MODEL &model ) 
  : m_input(input),
    m_output(output),
    m_model(model)
  {
  }
  int operator()()
  {
    CODE_VALUE high = MODEL::MAX_CODE;
    CODE_VALUE low = 0;
    CODE_VALUE value = 0;
    for ( int i = 0 ; i < MODEL::CODE_VALUE_BITS ; i++ ) {
      value <<= 1;
      value += m_input.get_bit() ? 1 : 0;
    }
    for ( ; ; ) {
      CODE_VALUE range = high - low + 1;
      CODE_VALUE scaled_value =  ((value - low + 1) * m_model.getCount() - 1 ) / range;
      int c;
      prob p = m_model.getChar( scaled_value, c );
      if ( c == 256 )
        break;
      m_output.putByte(c);

      high = low + (range*p.high)/p.count -1;
      low = low + (range*p.low)/p.count;

      for( ; ; ) {
        if ( high < MODEL::ONE_HALF ) {
        } else if ( low >= MODEL::ONE_HALF ) {
          value -= MODEL::ONE_HALF;
          low -= MODEL::ONE_HALF;
          high -= MODEL::ONE_HALF;
        } else if ( low >= MODEL::ONE_FOURTH && high < MODEL::THREE_FOURTHS ) {
          value -= MODEL::ONE_FOURTH;
          low -= MODEL::ONE_FOURTH;
          high -= MODEL::ONE_FOURTH;
        } else
          break;
        low <<= 1;
        high <<= 1;
        high++;
        value <<= 1;
        value += m_input.get_bit() ? 1 : 0;
      }
    }
    return 0;
  }
private :
  OUTPUT &m_output;
  INPUT &m_input;
  MODEL &m_model;
};

template<typename INPUT, typename OUTPUT, typename MODEL>
int decompress(INPUT &source, OUTPUT &target, MODEL &model)
{
  input_bits<INPUT> in(source,MODEL::CODE_VALUE_BITS);
  output_bytes<OUTPUT> out(target);
  decompressor<input_bits<INPUT>, output_bytes<OUTPUT>, MODEL> d(in,out, model);
  return d();
}

#endif
