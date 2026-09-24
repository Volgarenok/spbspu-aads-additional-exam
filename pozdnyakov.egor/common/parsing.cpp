#include "parsing.hpp"
#include <cctype>
#include <cstddef>
#include <limits>
#include <string>

namespace {
  bool isSpace(char symbol)
  {
    return std::isspace(static_cast< unsigned char >(symbol)) != 0;
  }

  bool isDigit(char symbol)
  {
    return std::isdigit(static_cast< unsigned char >(symbol)) != 0;
  }
}

void pozdnyakov::skipSpaces(const std::string & line, std::size_t & position)
{
  while (position < line.size() && isSpace(line[position])) {
    ++position;
  }
}

bool pozdnyakov::isLineEnd(const std::string & line, std::size_t position)
{
  skipSpaces(line, position);
  return position == line.size();
}

bool pozdnyakov::isBlank(const std::string & line)
{
  return isLineEnd(line, 0);
}

bool pozdnyakov::parseInteger(const std::string & line, std::size_t & position, int & result)
{
  constexpr long long base = 10;
  constexpr long long max_value = std::numeric_limits< int >::max();
  constexpr long long min_value = std::numeric_limits< int >::min();
  skipSpaces(line, position);
  bool negative = false;
  if (position < line.size() && (line[position] == '-' || line[position] == '+')) {
    negative = line[position] == '-';
    ++position;
  }
  if (position == line.size() || !isDigit(line[position])) {
    return false;
  }
  long long value = 0;
  while (position < line.size() && isDigit(line[position])) {
    value = value * base + (line[position] - '0');
    if (value > -min_value) {
      return false;
    }
    ++position;
  }
  if (position < line.size() && !isSpace(line[position])) {
    return false;
  }
  value = negative ? -value : value;
  if (value > max_value) {
    return false;
  }
  result = static_cast< int >(value);
  return true;
}

bool pozdnyakov::parseAccount(const std::string & line, std::size_t & position, int & account)
{
  int number = 0;
  if (!parseInteger(line, position, number) || number == std::numeric_limits< int >::min()) {
    return false;
  }
  account = number < 0 ? -number : number;
  return true;
}

std::string pozdnyakov::readToken(const std::string & line, std::size_t & position)
{
  skipSpaces(line, position);
  const std::size_t begin = position;
  while (position < line.size() && !isSpace(line[position])) {
    ++position;
  }
  return line.substr(begin, position - begin);
}
