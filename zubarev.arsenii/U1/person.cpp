#include "person.hpp"
void zubarev::init(PersonArray& arr)
{
  arr.data = nullptr;
  arr.size = 0;
  arr.capacity = 0;
}
void zubarev::destroy(PersonArray& arr)
{
  delete[] arr.data;
}
void zubarev::resize(PersonArray& arr)
{
  size_t new_cap = arr.capacity == 0 ? 8 : arr.capacity * 2;
  Person* new_data = new Person[new_cap];

  for (size_t i = 0; i < arr.size; ++i) {
    new_data[i] = std::move(arr.data[i]);
  }
  delete[] arr.data;

  arr.data = new_data;
  arr.capacity = new_cap;
}
void zubarev::push(PersonArray& arr, const Person& value)
{
  if (arr.size == arr.capacity) {
    resize(arr);
  }
  arr.data[arr.size] = value;
  ++arr.size;
}
bool zubarev::exists(const PersonArray& arr, size_t id)
{
  for (size_t i = 0; i < arr.size; ++i) {
    if (arr.data[i].id == id) {
      return true;
    }
  }

  return false;
}

void zubarev::init(HashSet& set)
{
  set.capacity = 1024;
  set.data = new size_t[set.capacity];
  set.used = new bool[set.capacity];

  for (size_t i = 0; i < set.capacity; ++i) {
    set.used[i] = false;
  }
}
void zubarev::destroy(HashSet& set)
{
  delete[] set.data;
  delete[] set.used;
}
size_t zubarev::hash(const HashSet& set, size_t value)
{
  return value % set.capacity;
}
bool zubarev::contains(const HashSet& set, size_t value)
{
  size_t pos = hash(set, value);

  for (size_t i = 0; i < set.capacity; ++i) {
    size_t cur = (pos + i) % set.capacity;
    if (!set.used[cur]) {
      return false;
    }
    if (set.data[cur] == value) {
      return true;
    }
  }
  return false;
}
void zubarev::insert(HashSet& set, size_t value)
{
  size_t pos = hash(set, value);

  for (size_t i = 0; i < set.capacity; ++i) {
    size_t cur = (pos + i) % set.capacity;
    if (!set.used[cur]) {
      set.data[cur] = value;
      set.used[cur] = true;
      return;
    }
  }
}
