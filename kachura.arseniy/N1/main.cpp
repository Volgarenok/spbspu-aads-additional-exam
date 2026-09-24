#include <iostream>
#include <fstream>
#include <string>

template<typename T>
bool readLine(T* line, int* from_ptr, int* to_ptr, int* value_ptr)
{
 *line >> *from_ptr;
 *line >> *to_ptr;
 *line >> *value_ptr;
  if(*from_ptr != *to_ptr && !(line -> eof()))
  {
    return true;
  }
  else
  {
    return false;
  }
}

int main(int argc, char* argv [])
{
  int from, to, value;
  size_t succesful_records, ignored_records = 0;
  std::string data;
  std::ifstream in ("input.txt");
  std::ofstream out ("output.txt");
  while(!(in.eof()))
  {
    if(readLine(&in, &from, &to, &value) == true)
    {
      succesful_records++;
      out << from << ' ' << to << ' ' << value << std::endl;
    }
    else
    {
      if(!(in.eof()))
      {
        ignored_records++;
      }
    }
  }
  std::cout << succesful_records << " " << ignored_records << '\n';
  in.close();
  out.close();
}
