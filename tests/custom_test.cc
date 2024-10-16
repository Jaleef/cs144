#include "../src/byte_stream.hh"
#include <string_view>
#include <iostream>
using namespace std;

int main() {
  ByteStream bs(2);
  Writer& w = bs.writer();
  Reader& r = bs.reader();

  w.push("");
  w.push("cat");
  w.push("");
  cout << r.peek()<< endl;
  cout << r.bytes_buffered()<< endl;
  cout << r.peek()<< endl;
  r.pop(1);
  w.push("");
  cout << r.peek()<< endl;
  cout << r.bytes_buffered()<< endl;
}