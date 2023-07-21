#ifndef BYTEIO_DOT_H
#define BYTEIO_DOT_H

#include <type_traits>

template<typename T, typename Enable = void>
class output_bytes
{
public :
  output_bytes(...) {};
public :
  void putByte(char){}
};

template<typename T>
class output_bytes<T,typename std::enable_if<std::is_base_of<std::ostream, T>::value>::type>
{
public :
  output_bytes(T &stream) : m_stream(stream){}
  void putByte(char c)
  {
    m_stream.put(c);
  }
private :
  T &m_stream;
};

template<>
class output_bytes<FILE *>
{
public :
  output_bytes(FILE *pFile)
  : m_pFile(pFile)
  {}
  void putByte(char c)
  {
    putc(c,m_pFile);
  }
private :
  FILE *m_pFile;
};

template<typename T,typename Enable = void>
class input_bytes
{
public :
  input_bytes(...) {};
public :
  int getByte();
};

template<typename T>
class input_bytes<T,typename std::enable_if<std::is_base_of<std::istream, T>::value>::type>
{
public :
  input_bytes(T &stream) 
  : m_stream(stream)
  {
  }
  int getByte()
  {
    return m_stream.get();
  }
private :
  T &m_stream;
};

template<>
class input_bytes<FILE *>
{
public :
  input_bytes(FILE *pFile) 
  : m_pFile( pFile )
  {}
  int getByte() {
    return getc(m_pFile);
  }
private :
  FILE *m_pFile;
};

#endif
