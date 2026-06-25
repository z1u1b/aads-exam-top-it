#include <cctype>
#include <fstream>
#include <stdexcept>
#include <functions.hpp>
#include <person.hpp>
#include "commands.hpp"

void zubarev::cmd_anons(std::istream&, std::ostream& out, AppState& state)
{
  size_t count = 0;
  for (size_t i = 0; i < state.persons.size; ++i) {
    if (!state.persons.data[i].has_info && !state.persons.data[i].is_deleted) {
      ++count;
    }
  }
  if (count == 0)
    return;

  size_t* temp = new size_t[count];
  size_t idx = 0;
  for (size_t i = 0; i < state.persons.size; ++i) {
    if (!state.persons.data[i].has_info && !state.persons.data[i].is_deleted) {
      temp[idx++] = state.persons.data[i].id;
    }
  }

  sort_ids(temp, count);
  for (size_t i = 0; i < count; ++i) {
    out << temp[i] << '\n';
  }
  delete[] temp;
}

void zubarev::cmd_deanon(std::istream& in, std::ostream&, AppState& state)
{
  size_t anon_id, id;
  if (!(in >> anon_id >> id))
    throw std::runtime_error("invalid");

  Person* p_anon = find_person(state.persons, anon_id);
  Person* p_id = find_person(state.persons, id);

  if (!p_anon || !p_id || p_anon->has_info || !p_id->has_info) {
    throw std::runtime_error("invalid");
  }

  p_anon->is_deleted = true;

  size_t write_idx = 0;
  for (size_t i = 0; i < state.meets.size; ++i) {
    if (state.meets.data[i].u == anon_id)
      state.meets.data[i].u = id;
    if (state.meets.data[i].v == anon_id)
      state.meets.data[i].v = id;

    if (state.meets.data[i].u != state.meets.data[i].v) {
      state.meets.data[write_idx++] = state.meets.data[i];
    }
  }
  state.meets.size = write_idx;
}

void zubarev::cmd_desc(std::istream& in, std::ostream& out, AppState& state)
{
  size_t id;
  if (!(in >> id))
    throw std::runtime_error("invalid");

  Person* p = find_person(state.persons, id);
  if (!p)
    throw std::runtime_error("invalid");

  if (p->has_info) {
    out << p->info << '\n';
  } else {
    out << "<ANON>\n";
  }
}

void zubarev::cmd_redesc(std::istream& in, std::ostream&, AppState& state)
{
  size_t id;
  if (!(in >> id))
    throw std::runtime_error("invalid");

  Person* p = find_person(state.persons, id);
  if (!p)
    throw std::runtime_error("invalid");

  char ch;
  while (in.get(ch) && std::isspace(static_cast< unsigned char >(ch)) && ch != '\n')
    ;
  if (ch != '"')
    throw std::runtime_error("invalid");

  std::string desc_str = "";
  while (in.get(ch) && ch != '"' && ch != '\n') {
    desc_str += ch;
  }
  if (ch != '"')
    throw std::runtime_error("invalid");

  p->info = desc_str;
  p->has_info = true;
}

void zubarev::cmd_meets(std::istream& in, std::ostream& out, AppState& state)
{
  size_t id;
  if (!(in >> id))
    throw std::runtime_error("invalid");
  if (!contains(state.ids, id) || find_person(state.persons, id) == nullptr) {
    throw std::runtime_error("invalid");
  }

  size_t count = 0;
  for (size_t i = 0; i < state.meets.size; ++i) {
    if (state.meets.data[i].u == id || state.meets.data[i].v == id) {
      ++count;
    }
  }
  if (count == 0)
    return;

  MeetView* views = new MeetView[count];
  size_t idx = 0;
  for (size_t i = 0; i < state.meets.size; ++i) {
    if (state.meets.data[i].u == id) {
      views[idx].other_id = state.meets.data[i].v;
      views[idx].duration = state.meets.data[i].duration;
      ++idx;
    } else if (state.meets.data[i].v == id) {
      views[idx].other_id = state.meets.data[i].u;
      views[idx].duration = state.meets.data[i].duration;
      ++idx;
    }
  }

  sort_meet_views(views, count);
  for (size_t i = 0; i < count; ++i) {
    out << views[i].other_id << " " << views[i].duration << '\n';
  }
  delete[] views;
}

static size_t get_unique_partners(zubarev::AppState& state, size_t id, size_t*& partners)
{
  size_t count = 0;
  for (size_t i = 0; i < state.meets.size; ++i) {
    if (state.meets.data[i].u == id || state.meets.data[i].v == id) {
      ++count;
    }
  }
  if (count == 0) {
    partners = nullptr;
    return 0;
  }
  partners = new size_t[count];
  size_t idx = 0;
  for (size_t i = 0; i < state.meets.size; ++i) {
    if (state.meets.data[i].u == id)
      partners[idx++] = state.meets.data[i].v;
    else if (state.meets.data[i].v == id)
      partners[idx++] = state.meets.data[i].u;
  }
  zubarev::sort_ids(partners, count);
  size_t unique_count = 0;
  for (size_t i = 0; i < count; ++i) {
    if (unique_count == 0 || partners[i] != partners[unique_count - 1]) {
      partners[unique_count++] = partners[i];
    }
  }
  return unique_count;
}

void zubarev::cmd_commons(std::istream& in, std::ostream& out, AppState& state)
{
  size_t id1, id2;
  if (!(in >> id1 >> id2))
    throw std::runtime_error("invalid");
  if (!contains(state.ids, id1) || !contains(state.ids, id2) || !find_person(state.persons, id1) ||
      !find_person(state.persons, id2)) {
    throw std::runtime_error("invalid");
  }

  size_t* p1 = nullptr;
  size_t* p2 = nullptr;
  size_t c1 = get_unique_partners(state, id1, p1);
  size_t c2 = get_unique_partners(state, id2, p2);

  size_t i = 0, j = 0;
  while (i < c1 && j < c2) {
    if (p1[i] == p2[j]) {
      out << p1[i] << '\n';
      ++i;
      ++j;
    } else if (p1[i] < p2[j]) {
      ++i;
    } else {
      ++j;
    }
  }
  delete[] p1;
  delete[] p2;
}

void zubarev::cmd_less(std::istream& in, std::ostream& out, AppState& state)
{
  size_t time_val, id;
  if (!(in >> time_val >> id))
    throw std::runtime_error("invalid");
  if (!contains(state.ids, id) || !find_person(state.persons, id))
    throw std::runtime_error("invalid");

  size_t count = 0;
  for (size_t i = 0; i < state.meets.size; ++i) {
    if ((state.meets.data[i].u == id || state.meets.data[i].v == id) && state.meets.data[i].duration < time_val) {
      ++count;
    }
  }
  if (count == 0)
    return;

  MeetView* views = new MeetView[count];
  size_t idx = 0;
  for (size_t i = 0; i < state.meets.size; ++i) {
    if ((state.meets.data[i].u == id || state.meets.data[i].v == id) && state.meets.data[i].duration < time_val) {
      views[idx].other_id = (state.meets.data[i].u == id) ? state.meets.data[i].v : state.meets.data[i].u;
      views[idx].duration = state.meets.data[i].duration;
      ++idx;
    }
  }

  sort_meet_views(views, count);
  for (size_t i = 0; i < count; ++i) {
    out << views[i].other_id << " " << views[i].duration << '\n';
  }
  delete[] views;
}

void zubarev::cmd_greater(std::istream& in, std::ostream& out, AppState& state)
{
  size_t time_val, id;
  if (!(in >> time_val >> id))
    throw std::runtime_error("invalid");
  if (!contains(state.ids, id) || !find_person(state.persons, id))
    throw std::runtime_error("invalid");

  size_t count = 0;
  for (size_t i = 0; i < state.meets.size; ++i) {
    if ((state.meets.data[i].u == id || state.meets.data[i].v == id) && state.meets.data[i].duration > time_val) {
      ++count;
    }
  }
  if (count == 0)
    return;

  MeetView* views = new MeetView[count];
  size_t idx = 0;
  for (size_t i = 0; i < state.meets.size; ++i) {
    if ((state.meets.data[i].u == id || state.meets.data[i].v == id) && state.meets.data[i].duration > time_val) {
      views[idx].other_id = (state.meets.data[i].u == id) ? state.meets.data[i].v : state.meets.data[i].u;
      views[idx].duration = state.meets.data[i].duration;
      ++idx;
    }
  }

  sort_meet_views(views, count);
  for (size_t i = 0; i < count; ++i) {
    out << views[i].other_id << " " << views[i].duration << '\n';
  }
  delete[] views;
}

void zubarev::cmd_out_persons(std::istream& in, std::ostream&, AppState& state)
{
  std::string filename;
  if (!(in >> filename))
    throw std::runtime_error("invalid");

  std::ofstream out_file(filename);
  if (!out_file)
    throw std::runtime_error("invalid");

  for (size_t i = 0; i < state.persons.size; ++i) {
    if (state.persons.data[i].has_info && !state.persons.data[i].is_deleted) {
      out_file << state.persons.data[i].id << " " << state.persons.data[i].info << '\n';
    }
  }
}
