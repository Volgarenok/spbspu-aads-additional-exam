#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv [])
{
  int from, to, value;
  size_t succesful_records, ignored_records;
  std::string data;
  std::ifstream in ("input.txt");
  std::ofstream out ("output.txt");
  for(size_t i = 0; in >> data; i++)
  {
    out << data;
    if (i == 2)
    {
      out << '\n';
    }
    else
    {
      out << ' ';
    }
  }
  in.close();
  out.close();
}
