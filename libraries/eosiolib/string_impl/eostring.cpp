// TODO:
// Consider: explicit, inline, noexcept

#include <cstring>   // memcpy, memset, strlen, strcpy
#include <limits>    // std::numeric_limits<size_t>::max()
#include <stdexcept> // std::out_of_range

#include <iostream> ///
using namespace std;

#include "eostring.hpp"

namespace eostring_impl {
   // size_t ratio(size_t n) {
   //    return n*1.618;
   // }
   
   char* allocate(const char* s, size_t n) {
      // assert(strlen(s) < n);
      char* begin = new char[n];
      // memset(begin, 42, 20);
      return begin;
   }
}

eostring::eostring()
   : _size{0}, _capacity{0}, _begin{nullptr}
{ }

eostring::eostring(const char* s)
   : _size{strlen(s)}, _capacity{_size*2} {
   assert(0 < _size);
   _begin = eostring_impl::allocate(s, _capacity);
   memcpy(_begin, s, strlen(s));
   _begin[strlen(s)] = '\0';
}

eostring::eostring(const char* s, size_t n)
   : _size{n}, _capacity{_size*2} {
   assert(0 < _size);
   _begin = eostring_impl::allocate(s, _capacity);
   memcpy(_begin, s, n);
   _begin[n] = '\0';
   
   // _begin = new char[_size+1];
   // memcpy(_begin, s, _size);
   // _begin[_size] = '\0';
}

eostring::eostring(size_t n, char c)
   : _size{n}, _capacity{_size} {
   if(_size) {
      _begin = new char[_size+1];
      memset(_begin, c, n);
      _begin[_size] = '\0';
   }
   else
      _begin = nullptr;
}

eostring::eostring(const eostring& s) {
   if(&s == this) return;
   if(s._size) {
      _size     = s._size;
      _capacity = s._capacity;
      _begin    = new char[s._size+1];
      
      memcpy(_begin, s._begin, _size);
      _begin[_size+1] = '\0';
   }
}

eostring::eostring(eostring&& s) {
   if(&s == this)
      return;
   if(s._size) {
      _size       = s._size;
      _capacity   = s._capacity;
      _begin      = s._begin;
   
      s._size     = 0;
      s._capacity = 0;
      s._begin    = nullptr;
   }
}

eostring::~eostring() {
   if(_size) {
      _size     = 0;
      _capacity = 0;
      delete[] _begin;
   }
}

eostring& eostring::operator=(const eostring& s) {
   if(&s == this)
      return *this;
   if(s._size) {
      _size     = s._size;
      _capacity = s._capacity;
      _begin    = new char[s._size+1];
      
      memcpy(_begin, s._begin, _size);
      _begin[_size+1] = '\0';
   }
   return *this;
}

eostring& eostring::operator=(eostring&& s)
{
   if(&s == this)
      return *this;
   if(s._size) {
      _size       = s._size;
      _capacity   = s._capacity;
      _begin      = s._begin;
      s._size     = 0;
      s._capacity = 0;
      s._begin    = nullptr;
   }
   return *this;
}

eostring& eostring::operator=(const char* s)
{
   if(strlen(s)) {
      _size     = strlen(s);
      _capacity = _size;
      _begin    = new char[_size+1];
      
      memcpy(_begin, s, _size);
      _begin[_size+1] = '\0';
   }
   return *this;
}

eostring& eostring::operator=(char c)
{
   if(c) {
      _size     = 1;
      _capacity = 1;
      _begin    = new char[_size+1];
      
      memcpy(_begin, &c, _size);
      _begin[_size+1] = '\0';
   }
   return *this;
}

eostring& eostring::operator+=(char c) {
   if(!_size) {
      _size = 2;
      _capacity = 2;
      _begin = new char[_size+1];
      memcpy(_begin, &c, 1);
      _begin[_size] = '\0';
   }
   
   if((c) && (_size < npos)) { // Does `c` really need to be valid?
      _begin[_size] = c;
      ++_size;
      _capacity = _size;

      char* begin{new char[_size+1]};
      memcpy(begin, _begin, _size);
      begin[_size] = '\0';
      
      delete[] _begin;
      _begin = begin;
   }
   return *this;
}

char& eostring::at(size_t n) {
   if(n < 0 || _size <= n)
      throw std::out_of_range("eostring::at()");
   return _begin[n];
}

const char& eostring::at(size_t n) const {
   if(n < 0 || _size <= n)
      throw std::out_of_range("eostring::at()");
   return _begin[n];
}

char& eostring::operator[](size_t n) {
   return _begin[n];
}

const char& eostring::operator[](size_t n) const {
   return _begin[n];
}

char& eostring::front() {
   return _begin[0];
}

const char& eostring::front() const {
   return _begin[0];
}

char& eostring::back() {
   return _begin[_size-1];
}

const char& eostring::back() const {
   return _begin[_size-1];
}

char* eostring::data() {
   return _begin;
}

const char* eostring::data() const {
   return _begin;
}

const char* eostring::c_str() const {
   return _begin;
}

eostring::iterator eostring::begin() {
   return &_begin[0];
}

eostring::const_iterator eostring::cbegin() const {
   return &_begin[0];
}

eostring::iterator eostring::end() {
   return &_begin[_size];
}

eostring::const_iterator eostring::cend() const {
   return &_begin[_size];
}

bool eostring::empty() const {
   return !_size;
}

size_t eostring::size() const {
   return _size;
}

size_t eostring::length() const {
   return _size;
}

size_t eostring::capacity() const {
   return _capacity;
}

size_t eostring::max_size() const {
   return npos;
}

bool operator< (const eostring& lhs, const eostring& rhs) {
   const char* beg_lhs{lhs._begin}; const char* end_lhs{lhs._begin + lhs._size};
   const char* beg_rhs{rhs._begin}; const char* end_rhs{rhs._begin + rhs._size};
   
   for (; beg_lhs != end_lhs && beg_rhs != end_rhs; ++beg_lhs, ++beg_rhs) {
      if (*beg_lhs < *beg_rhs)
         return true;
      if (*beg_rhs < *beg_lhs)
         return false;
   }
   
   return beg_lhs == end_lhs && beg_rhs != end_rhs;
}

bool operator> (const eostring& lhs, const eostring& rhs) {
   return (rhs < lhs);
}

bool operator<=(const eostring& lhs, const eostring& rhs) {
   return !(rhs < lhs);
}

bool operator>=(const eostring& lhs, const eostring& rhs) {
   return !(lhs < rhs);
}

bool operator==(const eostring& lhs, const eostring& rhs) {
   return !(lhs < rhs) && !(rhs < lhs);
}

bool operator!=(const eostring& lhs, const eostring& rhs) {
   return !(lhs == rhs);
}

void eostring::clear() {
   _size = 0;
   _capacity = 0;
   delete[] _begin;
}

void eostring::reserve(size_t n) {
   if(n >= _capacity)
      _capacity = n;
   else
      return;
}
void eostring::shrink_to_fit() {
   if(_capacity > _size) {
      _capacity = _size;
      char* begin{new char[_size+1]};
      memcpy(begin, _begin, _size);
      _begin[_size] = '\0';

      delete[] _begin;
      _begin = begin;
   }
   else
      return;
}

eostring& eostring::insert(size_t pos, const eostring& s) { // Look over this again
   assert(pos >= 0);
   assert(pos <= _size);
   
   char* begin = new char[_size + s._size]; // subtract `1` because there is only one null char
   _size += s._size;
   _capacity += _size;
   memcpy(begin, _begin, pos);
   memcpy(begin+pos, s._begin, s._size);

   delete[] _begin;
   _begin = begin;
   _begin[_size] = '\0';
   return *this;
}

eostring& eostring::erase(size_t pos, size_t len) { // Look over this again
   _size -= len;
   _capacity = _size;
   char* begin = new char[_size];
   memcpy(begin, _begin, pos);
   memcpy(begin+pos, _begin+pos+len, _size-len);
   delete[] _begin;
   _begin = begin;
   _begin[_size] = '\0';
   return *this;
}

void eostring::push_back(char c) {
   if(_size == _capacity) {
      ++_size;
      _capacity = _size;
      char* begin = new char[_size+1];
      memcpy(begin, _begin, _size-1);
      begin[_size-1] = c;
      begin[_size] = '\0';

      delete[] _begin;
      _begin = begin;
   }
}

void eostring::pop_back() {
   // for (int i = 0; i < 10; ++i) cout << _begin[i] << ' '; cout << endl;
   --_size;
   _capacity = _size;
   erase(_size, 0);
   _begin[_size] = '\0';
}

eostring& eostring::operator+=(const eostring& rhs) {
   for (size_t i{0}; i < rhs.size(); ++i)
      *this += rhs[i];
   return *this;
}

// eostring& eostring::replace(size_t pos,  size_t len,  const eostring& s) {
//    memcpy(_begin+len, s._begin, s.size());
//    cout << _begin << endl;
//    return *this;
// }

// eostring eostring::substr (size_t pos, size_t len) const {
//    char* begin{new char[len-pos]};
   
// }


// size_t copy (char* s, size_t len, size_t pos = 0) const {
//    memcpy(s, _begin+pos, len);
//    return len;
// }


//// void resize (size_t n)


//// void swap (eostring& str)



















// eostring& operator+=(eostring& lhs, const eostring& rhs) {
//    for (size_t i{0}; i < rhs.size(); ++i) {
//       lhs += rhs[i];
//    }
//    return lhs;
// }

// eostring operator+(const eostring& lhs, const eostring& rhs) {
//     eostring res{lhs};
//     res += rhs;
//     return res;
// }
