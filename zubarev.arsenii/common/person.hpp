#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
#include <iostream>

namespace zubarev
{
  struct Person
  {
    size_t id;
    std::string info;
    bool has_info;
    bool is_deleted;
  };

  struct PersonArray
  {
    Person* data;
    size_t size;
    size_t capacity;
  };

  struct Meet
  {
    size_t u;
    size_t v;
    size_t duration;
  };

  struct MeetArray
  {
    Meet* data;
    size_t size;
    size_t capacity;
  };

  struct HashSet
  {
    size_t* data;
    bool* used;
    size_t capacity;
  };

  struct AppState
  {
    PersonArray persons;
    MeetArray meets;
    HashSet ids;
  };

  struct MeetView
  {
    size_t other_id;
    size_t duration;
  };

  void init(PersonArray& arr);
  void destroy(PersonArray& arr);
  void resize(PersonArray& arr);
  void push(PersonArray& arr, const Person& value);
  Person* find_person(PersonArray& arr, size_t id);

  void init(MeetArray& arr);
  void destroy(MeetArray& arr);
  void resize(MeetArray& arr);
  void push(MeetArray& arr, const Meet& value);

  void init(HashSet& set);
  void destroy(HashSet& set);
  size_t hash(const HashSet& set, size_t value);
  bool contains(const HashSet& set, size_t value);
  void insert(HashSet& set, size_t value);

  void sort_ids(size_t* arr, size_t size);
  void sort_meet_views(MeetView* arr, size_t size);
}

#endif
