// TODO:
// Check uses of `.data()` vs `._begin`
// TODO:
// Check the null terminator
// TODO:
// `cbegin` and `cend`
// Test with `static const eostring` as well
// TODO:
// Ask Bucky if the `c_str()` function is a good practice
// TODO:
// Make sure memset functionality is working as planned
// TODO:
// Check previous `memcpy` and if it should be added back in there
// TODO:
// Check cases of unique_ptr<char[]> in the tests to see if those cases are tested adequately
// TODO:
// Remeber the null char

/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#pragma once

#include <cstring> // memcpy, memset, strlen

#include "datastream.hpp" // eosio::datastream
#include "varint.hpp"     // eosio::unsigned_int

namespace eosio {

   class eostring
   {
   public:
      static constexpr size_t npos = -1;

      template <size_t N>
      constexpr eostring(const char (&str)[N]) : _size{N-1}, _capacity{_size}, _begin{str}
      { }

      constexpr eostring() : _size{0}, _capacity{0}, _begin{""}
      { }
      
      constexpr eostring(const size_t n, const char c) : _size{n}, _capacity{_size*2} {
         char* begin{new char[_capacity]}; // Polish this up
         memset(begin, c, _size);
         _begin = begin;
      }
      
      constexpr eostring(const eostring& str, const size_t pos, const size_t n) : _size{n}, _capacity{_size*2} {
         if (n ==  eostring::npos || str._size < pos+n) {
            _size     = str._size;
            _capacity = _size*2;
         }
    
         clone(_size, _capacity, str.data()+pos);
      }

      constexpr eostring(const eostring& str) {
         _size     = str._size;
         _capacity = str._capacity;
         
         if (is_sso())
            _begin = std::get<const char*>(str._begin);
         else
            clone(str._size, str._capacity, str.data());     
      }

      constexpr eostring(eostring&& str) {
         _size     = str._size;
         _capacity = str._capacity;
         
         if (is_sso())
            _begin = std::get<const char*>(str._begin);
         else
            _begin = std::move(std::get<uptr>(str._begin));
      }
    
      eostring& operator=(const eostring& str) {
         if (&str == this)
            return *this;
    
         _size     = str._size;
         _capacity = str._capacity;

         if (is_sso())
            _begin = std::get<const char*>(str._begin);
         else
            clone(_size, _capacity, str.data());
    
         return *this;
      }

      eostring& operator=(eostring&& str) {
         if (&str == this)
            return *this;
         
         _size     = str._size;
         _capacity = str._capacity;
         
         if (is_sso())
            _begin = std::get<const char*>(str._begin);
         else
            _begin = std::move(std::get<uptr>(str._begin));
    
         return *this;
      }

      eostring& operator=(const char* str) {
         eosio::check(str != nullptr, "eostring::operator=");
         
         _size     = strlen(str);
         _capacity = _size;
         _begin    = str;
    
         return *this;
      }

      char& operator[](const size_t n) {
         if (is_sso())
            clone(_size, _capacity, std::get<const char*>(_begin));
         return std::get<uptr>(_begin).get()[n];
      }

      const char operator[](const size_t n) const {
         if (is_sso())
            return std::get<const char*>(_begin)[n];
         else
            return std::get<uptr>(_begin).get()[n];
      }

      char& at(const size_t n) {
         eosio::check(0 <= n && n < _size, "eostring::at");
         return operator[](n);
      }

      const char at(const size_t n) const {
         eosio::check(0 <= n && n < _size, "eostring::at const");
         return operator[](n);
      }

      char& front() {
         return at(0);
      }

      const char front() const {
         return at(0);
      }

      char& back() {
         return at(_size-1);
      }

      const char back() const {
         return at(_size-1);
      }

      char* data() {
         return begin();
      }

      const char* data() const {
         return cbegin();
      }

      const char* c_str() const {
         char* str = new char[_size+1];
         
         if (is_sso())
            memcpy(str, std::get<const char*>(_begin), _size);
         else
            memcpy(str, std::get<uptr>(_begin).get(), _size);
         
         str[_size] = '\0';
         return str;
      }

      char* begin() {
         if (is_sso()) {
            _capacity *= 2;
            clone(_size, _capacity, std::get<const char*>(_begin));
         }
         return std::get<uptr>(_begin).get();
      }

      const char* cbegin() const {
         if (is_sso())
            return std::get<const char*>(_begin);
         return std::get<uptr>(_begin).get();
      }

      char* end() {
         return begin()+_size;
      }

      const char* cend() const {
         return cbegin()+_size;
      }

      bool empty() const {
         return !_size;
      }

      size_t size() const {
         return _size;
      }

      size_t length() const {
         return _size;
      }

      size_t capacity() const {
         return _capacity;
      }

      size_t max_size() const {
         return npos;
      }
   
      void reserve(const size_t n) {
         if (_capacity < n) {
            _capacity = n;

            if (is_sso())
               clone(_size, _capacity, std::get<const char*>(_begin));
            else
               clone(_size, _capacity, std::get<uptr>(_begin).get());
         }
         else
            return;
      }

      void shrink_to_fit() {
         _capacity = _size;
      }

      void clear() {
         _size = 0;

         if (is_sso())
            clone(_size, _capacity, std::get<const char*>(_begin));
         std::get<uptr>(_begin).get()[0] = '\0';
      }

      void resize(const size_t n) {
         _size     = n;
         _capacity = _size*2;
         
         if (is_sso())
            clone(_size, _capacity, std::get<const char*>(_begin));
         clone(_size, _capacity, std::get<uptr>(_begin).get());
      }

      void swap(eostring& str) {
         eostring temp = *this;
    
         *this = str;
         str   = temp;
      }

      void push_back(const char c) {
         *this += c;
      }

      void pop_back() {
         if (_size == 0)
            return;
         --_size;
      }

      eostring substr(size_t pos = 0, size_t len = npos) const {
         return eostring(*this, pos, len);
      }

      size_t copy(char* s, size_t len, size_t pos = 0) const {
         eosio::check(pos <= _size, "eostring::copy");
         len = (_size < pos+len) ? _size : len;
         
         if (is_sso())
            memcpy(s, std::get<const char*>(_begin)+pos, len);
         else
            memcpy(s, std::get<uptr>(_begin).get()+pos, len);
         
         return (_size < pos+len) ? _size-pos : len;
      }

      eostring& insert(const size_t pos, const char* str) {
         return insert(pos, str, strlen(str));
      }
      
      eostring& insert(const size_t pos, const char* str, const size_t len) {
         eosio::check(str != nullptr, "eostring::insert");
         eosio::check(0 <= pos && pos <= _size, "eostring::insert");

         if ( _capacity < (_size+len)) {
            _size      += len;
            _capacity  = _size*2;
            uptr begin = std::make_unique<char[]>(_capacity);
            
            memcpy(begin.get(), std::get<const char*>(_begin), pos);
            memcpy(begin.get()+pos, str, len);
            memcpy(begin.get()+len+pos, std::get<const char*>(_begin)+pos, _size-len-pos);
            
            _begin = std::move(begin);
         }
         else {
            _size += len;
            memmove(std::get<uptr>(_begin).get()+pos+len, std::get<uptr>(_begin).get()+pos, _size-pos);
            memcpy(std::get<uptr>(_begin).get()+pos, str, len);
         }
         
         return *this;
      }

      eostring& insert(const size_t pos, const eostring& str) {
         eosio::check(0 <= pos && pos <= _size, "eostring::insert");

         insert(pos, str.c_str());
    
         return *this;
      }

      eostring& erase(size_t pos = 0, size_t len = npos) {
         eosio::check(0 <= pos && pos <= _size, "eostring::erase");

         if(len == eostring::npos)
            len = _size-pos;
    
         _size -= len;

         if (is_sso())
            clone(_size, _capacity, std::get<const char*>(_begin));
         memmove(std::get<uptr>(_begin).get()+pos+len, std::get<uptr>(_begin).get(), len);
    
         return *this;
      }

      eostring& append(const char* str) {
         eosio::check(str != nullptr, "eostring::append");
         insert(_size, str);
    
         return *this;
      }
    
      eostring& append(const eostring& str) {
         insert(_size, str);
         return *this;
      }

      eostring& operator+=(const char c) {
         if (_capacity == 0) {
            _size     = 1;
            _capacity = 2;
            clone(1, _capacity, &c);
         }
         else if (_size == _capacity) {
            _capacity = ++_size*2;
            if (is_sso()) {
               clone(_size, _capacity, std::get<const char*>(_begin));
               std::get<uptr>(_begin).get()[_size-1] = c;
            }  
            else {
               clone(_size, _capacity, std::get<uptr>(_begin).get());
               std::get<uptr>(_begin).get()[_size-1] = c;
            }
         }
         else
            std::get<uptr>(_begin).get()[_size++] = c;
         
         return *this;
      }

      eostring& operator+=(const char* rhs) {
         for (size_t i{0}; i < strlen(rhs); ++i) {
            *this += rhs[i];
         }
    
         return *this;
      }

      eostring& operator+=(const eostring& rhs) {
         for (size_t i{0}; i < rhs.size(); ++i) {
            *this += rhs[i];
         }
    
         return *this;
      }
   
      friend bool operator< (const eostring& lhs, const eostring& rhs);
      friend bool operator> (const eostring& lhs, const eostring& rhs);
      friend bool operator<=(const eostring& lhs, const eostring& rhs);
      friend bool operator>=(const eostring& lhs, const eostring& rhs);
      friend bool operator==(const eostring& lhs, const eostring& rhs);
      friend bool operator!=(const eostring& lhs, const eostring& rhs);
   
      friend eostring operator+ (const eostring& lhs, const eostring& rhs);

   public:
      using uptr    = std::unique_ptr<char[]>;
      using sso_str = std::variant<const char*, uptr>;
      
      size_t _size     = 0;
      size_t _capacity = 0;
      sso_str _begin   = nullptr;

      void clone(size_t size, size_t capacity, const char* str) {
         _size       = size;
         _capacity   = capacity;
         uptr begin  = std::make_unique<char[]>(capacity);
         
         memcpy(begin.get(), str, size);
         _begin = std::move(begin);
      }

      constexpr bool is_sso() const {
         return (std::holds_alternative<const char*>(_begin)) ? true : false;
      }
   };

   bool operator< (const eostring& lhs, const eostring& rhs) {
      const char* beg_lhs{lhs.cbegin()}; const char* end_lhs{lhs.cend()};
      const char* beg_rhs{rhs.cbegin()}; const char* end_rhs{rhs.cend()};
   
      for(; beg_lhs != end_lhs && beg_rhs != end_rhs; ++beg_lhs, ++beg_rhs) {
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

   eostring operator+(const eostring& lhs, const eostring& rhs) {
      eostring res{lhs};
      res += rhs;
      return res;
   }
   
   // template<typename DataStream>
   // DataStream& operator<<(DataStream& ds, const eostring& str) {
   //    ds << unsigned_int(str.size());
   //    if (str.size())
   //       ds.write(str.data(), str.size());
   //    return ds;
   // }
   
   // template<typename DataStream>
   // DataStream& operator>>(DataStream& ds, eostring& str) {
   //    unsigned_int size;
   //    ds >> size;
   //    str.insert(0, ds.pos(), size.value);
   //    ds.seekp(size.value);      
   //    return ds;
   // }
   
} // namespace eosio
