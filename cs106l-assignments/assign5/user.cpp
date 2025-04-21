#include "user.h"

/**
 * Creates a new User with the given name and no friends.
 */
User::User(const std::string& name)
  : _name(name)
  , _friends(nullptr)
  , _size(0)
  , _capacity(0)
{
}

/**
 * Adds a friend to this User's list of friends.
 * @param name The name of the friend to add.
 */
void
User::add_friend(const std::string& name)
{
  if (_size == _capacity) {
    _capacity = 2 * _capacity + 1;
    std::string* newFriends = new std::string[_capacity];
    for (size_t i = 0; i < _size; ++i) {
      newFriends[i] = _friends[i];
    }
    delete[] _friends;
    _friends = newFriends;
  }

  _friends[_size++] = name;
}

/**
 * Returns the name of this User.
 */
std::string
User::get_name() const
{
  return _name;
}

/**
 * Returns the number of friends this User has.
 */
size_t
User::size() const
{
  return _size;
}

/**
 * Sets the friend at the given index to the given name.
 * @param index The index of the friend to set.
 * @param name The name to set the friend to.
 */
void User::set_friend(size_t index, const std::string& name)
{
  _friends[index] = name;
}

/** 
 * STUDENT TODO:
 * The definitions for your custom operators and special member functions will go here!
 */



 void
 User::add_friend(const std::string& name)
 {
   if (_size == _capacity) {
     _capacity = 2 * _capacity + 1;
     std::string* newFriends = new std::string[_capacity];
     for (size_t i = 0; i < _size; ++i) {
       newFriends[i] = _friends[i];
     }
     delete[] _friends;
     _friends = newFriends;
   }
 
   _friends[_size++] = name;
 }
 
 std::string* User::get_friend() const {
    std::string* friendlist = new std::string[_size];
    for (size_t i = 0;i < _size;++i) {
      friendlist[i] = _friends[i];
    }
    return friendlist;
 }
 
 friend std::ostream &operator << (std::ostream & os,const User &usr) {
    os << "User(";
    os << "name="
    os << usr.get_name();
    os << ", friends=[";
    std::string* friendlist = usr.get_friend();
    int siz = usr.size();
    for (size_t i = 0;i + 1 < siz;++i) {
      os << friendlist[i] << ", ";
    }
    if (siz) os << friendlist[siz - 1];
    os << "])";
 }