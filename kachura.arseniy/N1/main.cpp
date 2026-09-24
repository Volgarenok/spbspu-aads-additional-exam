#include <iostream>
#include <fstream>
#include <string>

struct Transaction 
{
  static size_t succesful_records, ignored_records;
  int from, to, value;
};

int main(int argc, char* argv [])
{
  std::string line;
  std::ifstream in ("input.txt");
  std::ofstream out;
  out.open("output.txt");
  while(std::getline(in, line))
  {
    out << line << std::endl;
  }
  out.close();
}
