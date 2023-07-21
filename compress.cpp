#include <iostream>
#include <fstream>

#include "practice/modelA.h"
#include "practice/compressor.h"

using namespace std;

int main(int argc, char* argv[])
{
  if ( argc < 3 ) {
    cerr << "missing command line arguments\n";
    return 255;
  }
  ifstream input(argv[1], ifstream::binary);
  ofstream output(argv[2], ofstream::binary);
  modelA<int, 16, 14> cmodel;
  compress(input, output, cmodel);
  return 0;
}
