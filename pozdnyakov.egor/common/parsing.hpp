#ifndef PARSING_HPP
#define PARSING_HPP

#include <cstddef>
#include <string>

namespace pozdnyakov {
  void skipSpaces(const std::string & line, std::size_t & position);
  bool isLineEnd(const std::string & line, std::size_t position);
  bool isBlank(const std::string & line);
  bool parseInteger(const std::string & line, std::size_t & position, int & result);
  bool parseAccount(const std::string & line, std::size_t & position, int & account);
  std::string readToken(const std::string & line, std::size_t & position);
}

#endif
