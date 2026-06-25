#include <cctype>
#include "functions.hpp"

bool zubarev::detail::parse_args(int argc, char** argv, std::string& in_name, std::string& out_name)
{
  bool has_in = false;
  bool has_out = false;

  for (int i = 1; i < argc; ++i) {
    std::string arg(argv[i]);
    if (arg.find("in:") == 0) {
      if (has_in || arg.size() <= 3) {
        return false;
      }
      in_name = arg.substr(3);
      has_in = true;
    } else if (arg.find("out:") == 0) {
      if (has_out || arg.size() <= 4) {
        return false;
      }
      out_name = arg.substr(4);
      has_out = true;
    } else {
      return false;
    }
  }
  return true;
}
