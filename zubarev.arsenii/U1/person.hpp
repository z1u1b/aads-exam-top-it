#ifndef PERSON_HPP
#define PERSON_HPP

#include <iostream>
namespace zubarev
{
  struct Person
  {
    size_t id;
    std::string info;
  };

  struct PersonArray
  {
    Person* data;
    size_t size;
    size_t capacity;
  };

  struct HashSet
  {
    size_t* data;
    bool* used;
    size_t capacity;
  };

  void init(PersonArray& arr);
  void destroy(PersonArray& arr);
  void resize(PersonArray& arr);
  void push(PersonArray& arr, const Person& value);
  bool exists(const PersonArray&, size_t);

  void init(HashSet& set);
  void destroy(HashSet& set);
  size_t hash(const HashSet& set, size_t value);
  bool contains(const HashSet& set, size_t value);
  void insert(HashSet& set, size_t value);

}

#endif
