#include <iostream>
#include <fstream>
#include <string>

template<typename T>
void readLine(T* line, int* from_ptr, int* to_ptr, int* value_ptr)
{
  for(size_t i = 0; i < 3; i++)
  {
    *line >> *from_ptr;
    *line >> *to_ptr;
    *line >> *value_ptr;
  }
}

bool checkAccount(int* from_ptr, int* to_ptr, int* value_ptr)
{
  if(*from_ptr != *to_ptr)
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
  to = 0;
  value = 0;
  size_t succesful_records, ignored_records;
  std::string data;
  std::ifstream in ("input.txt");
  std::ofstream out ("output.txt");
  readLine(&in, &from, &to, &value);
  if(checkAccount(&from, &to, &value) == true)
  {
    out << from << ' ' << to << ' ' << value << '\n';
  }
  in.close();
  out.close();
}
