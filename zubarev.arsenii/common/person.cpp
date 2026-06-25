#include "person.hpp"
#include <utility>

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

zubarev::Person* zubarev::find_person(PersonArray& arr, size_t id)
{
  for (size_t i = 0; i < arr.size; ++i) {
    if (arr.data[i].id == id && !arr.data[i].is_deleted) {
      return &arr.data[i];
    }
  }
  return nullptr;
}

void zubarev::init(MeetArray& arr)
{
  arr.data = nullptr;
  arr.size = 0;
  arr.capacity = 0;
}

void zubarev::destroy(MeetArray& arr)
{
  delete[] arr.data;
}

void zubarev::resize(MeetArray& arr)
{
  size_t new_cap = arr.capacity == 0 ? 8 : arr.capacity * 2;
  Meet* new_data = new Meet[new_cap];

  for (size_t i = 0; i < arr.size; ++i) {
    new_data[i] = arr.data[i];
  }
  delete[] arr.data;

  arr.data = new_data;
  arr.capacity = new_cap;
}

void zubarev::push(MeetArray& arr, const Meet& value)
{
  if (arr.size == arr.capacity) {
    resize(arr);
  }
  arr.data[arr.size] = value;
  ++arr.size;
}

void zubarev::init(HashSet& set)
{
  set.capacity = 2048; // Увеличим запас для интерактивных данных
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

void zubarev::sort_ids(size_t* arr, size_t size)
{
  for (size_t i = 1; i < size; ++i) {
    size_t key = arr[i];
    long long j = static_cast< long long >(i) - 1;
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      --j;
    }
    arr[j + 1] = key;
  }
}

void zubarev::sort_meet_views(MeetView* arr, size_t size)
{
  for (size_t i = 1; i < size; ++i) {
    MeetView key = arr[i];
    long long j = static_cast< long long >(i) - 1;
    while (j >= 0 &&
           (arr[j].other_id > key.other_id || (arr[j].other_id == key.other_id && arr[j].duration > key.duration))) {
      arr[j + 1] = arr[j];
      --j;
    }
    arr[j + 1] = key;
  }
}
