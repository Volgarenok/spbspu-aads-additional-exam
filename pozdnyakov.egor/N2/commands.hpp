#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <ostream>
#include <string>
#include <tree.hpp>
#include "bank-data.hpp"

namespace pozdnyakov {
  using Command = bool (*)(BankData & data, const std::string & arguments, std::ostream & out);
  using CommandTree = Tree< std::string, Command >;

  void makeCommands(CommandTree & commands);
  void runCommand(const CommandTree & tree, BankData & data, const std::string & line, std::ostream & out);
}

#endif
