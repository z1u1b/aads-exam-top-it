#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <functions.hpp>
#include <person.hpp>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  std::string in_name = "";
  std::string data_name = "";

  if (!zubarev::detail::parse_args_meet(argc, argv, in_name, data_name)) {
    std::cerr << "Invalid command line arguments\n";
    return 1;
  }

  zubarev::AppState state;
  init(state.persons);
  init(state.meets);
  init(state.ids);

  if (!in_name.empty()) {
    std::ifstream in_file(in_name);
    if (!in_file) {
      std::cerr << "Failed to open input file\n";
      destroy(state.persons);
      destroy(state.meets);
      destroy(state.ids);
      return 2;
    }

    std::string line;
    while (std::getline(in_file, line)) {
      size_t pos = 0;
      while (pos < line.size() && std::isspace(static_cast< unsigned char >(line[pos])))
        ++pos;
      if (pos == line.size())
        continue;

      size_t id = 0;
      bool hasId = false;
      while (pos < line.size() && std::isdigit(static_cast< unsigned char >(line[pos]))) {
        hasId = true;
        id = id * 10 + (line[pos] - '0');
        ++pos;
      }
      if (!hasId)
        continue;

      if (pos < line.size() && !std::isspace(static_cast< unsigned char >(line[pos])))
        continue;
      while (pos < line.size() && std::isspace(static_cast< unsigned char >(line[pos])))
        ++pos;

      if (pos == line.size())
        continue;

      std::string info = line.substr(pos);
      if (info.empty())
        continue;

      if (contains(state.ids, id))
        continue;

      zubarev::Person person;
      person.id = id;
      person.info = info;
      person.has_info = true;
      person.is_deleted = false;

      push(state.persons, person);
      insert(state.ids, id);
    }
  }

  std::ifstream data_file(data_name);
  if (!data_file) {
    std::cerr << "Failed to open data file\n";
    destroy(state.persons);
    destroy(state.meets);
    destroy(state.ids);
    return 2;
  }

  bool has_valid_data = false;
  std::string line;

  while (std::getline(data_file, line)) {
    size_t pos = 0;
    while (pos < line.size() && std::isspace(static_cast< unsigned char >(line[pos])))
      ++pos;
    if (pos == line.size())
      continue;

    size_t id1 = 0;
    bool has_id1 = false;
    while (pos < line.size() && std::isdigit(static_cast< unsigned char >(line[pos]))) {
      has_id1 = true;
      id1 = id1 * 10 + (line[pos] - '0');
      ++pos;
    }
    if (!has_id1 || pos == line.size() || !std::isspace(static_cast< unsigned char >(line[pos]))) {
      std::cerr << "Error parsing meeting data\n";
      destroy(state.persons);
      destroy(state.meets);
      destroy(state.ids);
      return 3;
    }
    while (pos < line.size() && std::isspace(static_cast< unsigned char >(line[pos])))
      ++pos;

    size_t id2 = 0;
    bool has_id2 = false;
    while (pos < line.size() && std::isdigit(static_cast< unsigned char >(line[pos]))) {
      has_id2 = true;
      id2 = id2 * 10 + (line[pos] - '0');
      ++pos;
    }
    if (!has_id2 || pos == line.size() || !std::isspace(static_cast< unsigned char >(line[pos]))) {
      std::cerr << "Error parsing meeting data\n";
      destroy(state.persons);
      destroy(state.meets);
      destroy(state.ids);
      return 3;
    }
    while (pos < line.size() && std::isspace(static_cast< unsigned char >(line[pos])))
      ++pos;

    size_t duration = 0;
    bool has_dur = false;
    while (pos < line.size() && std::isdigit(static_cast< unsigned char >(line[pos]))) {
      has_dur = true;
      duration = duration * 10 + (line[pos] - '0');
      ++pos;
    }
    if (!has_dur || (pos != line.size() && !std::isspace(static_cast< unsigned char >(line[pos])))) {
      std::cerr << "Error parsing meeting data\n";
      destroy(state.persons);
      destroy(state.meets);
      destroy(state.ids);
      return 3;
    }

    has_valid_data = true;

    if (!contains(state.ids, id1)) {
      zubarev::Person p;
      p.id = id1;
      p.info = "";
      p.has_info = false;
      p.is_deleted = false;
      push(state.persons, p);
      insert(state.ids, id1);
    }
    if (!contains(state.ids, id2)) {
      zubarev::Person p;
      p.id = id2;
      p.info = "";
      p.has_info = false;
      p.is_deleted = false;
      push(state.persons, p);
      insert(state.ids, id2);
    }

    if (id1 == id2)
      continue;

    zubarev::Meet meet;
    meet.u = id1;
    meet.v = id2;
    meet.duration = duration;
    push(state.meets, meet);
  }
  data_file.close();

  if (!has_valid_data) {
    std::cerr << "Data file is entirely empty\n";
    destroy(state.persons);
    destroy(state.meets);
    destroy(state.ids);
    return 3;
  }

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (cmd == "anons")
        zubarev::cmd_anons(std::cin, std::cout, state);
      else if (cmd == "deanon")
        zubarev::cmd_deanon(std::cin, std::cout, state);
      else if (cmd == "desc")
        zubarev::cmd_desc(std::cin, std::cout, state);
      else if (cmd == "redesc")
        zubarev::cmd_redesc(std::cin, std::cout, state);
      else if (cmd == "meets")
        zubarev::cmd_meets(std::cin, std::cout, state);
      else if (cmd == "commons")
        zubarev::cmd_commons(std::cin, std::cout, state);
      else if (cmd == "less")
        zubarev::cmd_less(std::cin, std::cout, state);
      else if (cmd == "greater")
        zubarev::cmd_greater(std::cin, std::cout, state);
      else if (cmd == "out-persons")
        zubarev::cmd_out_persons(std::cin, std::cout, state);
      else
        throw std::runtime_error("unknown");
    } catch (...) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  destroy(state.persons);
  destroy(state.meets);
  destroy(state.ids);
  return 0;
}
