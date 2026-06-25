#ifndef FUNC_HPP
#define FUNC_HPP

#include <string>
namespace zubarev
{
  namespace detail
  {
    bool parse_args(int argc, char* argv[], std::string& inputName, std::string& outputName);
    bool parse_args_meet(int argc, char** argv, std::string& in_name, std::string& out_name);

  }
}

#endif
