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

/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#pragma once

#include <cstring> // memcpy, memset, strlen

#include "datastream.hpp" // eosio::datastream
// #include "datastream.hpp" // eosio::datastream
#include "varint.hpp"     // eosio::unsigned_int

namespace eosio {

   class eostring
   {
   public:
      static constexpr size_t npos = -1;

      // template <typename T, typename = std::enable_if_t<
      //                          std::is_same_v< std::remove_const_t< std::remove_pointer_t< std::remove_reference_t<T>>>, char>, int>>
      // constexpr eostring(T&& str) : _size{strlen(str)}, _capacity{_size} {
      //    clone(_size, _capacity, str);
      // }

      template <size_t N>
      constexpr eostring(const char (&str)[N]) : _size{N-1}, _capacity{_size}, _begin{str}
      { }

      constexpr eostring() : _size{0}, _capacity{0}, _begin{""}
      { }
      
      constexpr eostring(const size_t n, const char c) : _size{n}, _capacity{_size*2} {
         _begin = expand_mset(_size, _capacity, c);
      }
      
      constexpr eostring(const eostring& str, const size_t pos, const size_t n) : _size{n}, _capacity{_size*2} {
         if(n ==  eostring::npos || str._size < pos+n) {
            _size     = str._size;
            _capacity = _size*2;
         }
    
         clone(_size, _capacity, str.data()+pos);
      }

      constexpr eostring(const eostring& str) {
         _size     = str._size;
         _capacity = str._capacity;
         
         if(std::holds_alternative<const char*>(str._begin))
            _begin = std::get<const char*>(str._begin);
         else
            clone(str._size, str._capacity, str.data());     
      }

      constexpr eostring(eostring&& str) {
         _size     = str._size;
         _capacity = str._capacity;
         
         if(std::holds_alternative<const char*>(str._begin))
            _begin = std::get<const char*>(str._begin);
         else
            _begin = std::move(std::get<std::unique_ptr<char[]>>(str._begin));
      }
    
      eostring& operator=(const eostring& str) {
         if(&str == this)
            return *this;
    
         _size     = str._size;
         _capacity = str._capacity;

         if(std::holds_alternative<const char*>(str._begin))
            _begin = std::get<const char*>(str._begin);
         else
            clone(_size, _capacity, str.data());
    
         return *this;
      }

      eostring& operator=(eostring&& str) {
         if(&str == this)
            return *this;
         
         _size     = str._size;
         _capacity = str._capacity;
         
         if(std::holds_alternative<const char*>(str._begin))
            _begin = std::get<const char*>(str._begin);
         else
            _begin = std::move(std::get<std::unique_ptr<char[]>>(str._begin));
    
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
         if(std::holds_alternative<const char*>(_begin))
            clone(_size, _capacity, std::get<const char*>(_begin));
         return std::get<std::unique_ptr<char[]>>(_begin).get()[n];
      }

      const char operator[](const size_t n) const {
         if(std::holds_alternative<const char*>(_begin))
            return std::get<const char*>(_begin)[n];
         else
            return std::get<std::unique_ptr<char[]>>(_begin).get()[n];
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
         
         if (std::holds_alternative<const char*>(_begin))
            memcpy(str, std::get<const char*>(_begin), _size);
         else
            memcpy(str, std::get<std::unique_ptr<char[]>>(_begin).get(), _size);
         
         str[_size] = '\0';
         return str;
      }

      char* begin() {
         if (std::holds_alternative<const char*>(_begin)) {
            _capacity *= 2;
            clone(_size, _capacity, std::get<const char*>(_begin));
         }
         return std::get<std::unique_ptr<char[]>>(_begin).get();
      }

      const char* cbegin() const {
         if (std::holds_alternative<const char*>(_begin))
            return std::get<const char*>(_begin);
         return std::get<std::unique_ptr<char[]>>(_begin).get();
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
         if(_capacity < n) {
            _capacity = n;

            if(std::holds_alternative<const char*>(_begin))
               clone(_size, _capacity, std::get<const char*>(_begin));
            else
               clone(_size, _capacity, std::get<std::unique_ptr<char[]>>(_begin).get());
         }
         else
            return;
      }

      void shrink_to_fit() {
         _capacity = _size;
      }

      void clear() {
         _size     = 0;

         if(std::holds_alternative<const char*>(_begin))
            clone(_size, _capacity, std::get<const char*>(_begin));
         std::get<std::unique_ptr<char[]>>(_begin).get()[0] = '\0';
      }

      void resize(const size_t n) {
         _size     = n;
         _capacity = _size*2;
         
         if(std::holds_alternative<const char*>(_begin))
            clone(_size, _capacity, std::get<const char*>(_begin));
         clone(_size, _capacity, std::get<std::unique_ptr<char[]>>(_begin).get());
      }

      void swap(eostring& str) {
         eostring temp = *this;
    
         *this = str;
         str   = temp;
      }

      // void push_back(const char c) {
      //    *this += c;
      // }

      void pop_back() {
         if(_size == 0)
            return;
         --_size;
      }

      eostring substr(size_t pos = 0, size_t len = npos) const {
         return eostring(*this, pos, len);
      }

      size_t copy(char* s, size_t len, size_t pos = 0) const {
         memcpy(s, substr(pos, len)._begin, substr(pos, len)._size);
         
         // _size        = substr(pos, len)._size
         // _capacity is = ...
         
         clone(_size, _capacity, std::get<const char*>(_begin));
         return (_size < pos+len) ? _size-pos : len;
      }

      // eostring& insert(const size_t pos, const char* str) {
      //    return insert(pos, str, strlen(str));
      // }
      
      // eostring& insert(const size_t pos, const char* str, const size_t len) {
      //    eosio::check(str != nullptr, "eostring::insert");
      //    eosio::check(0 <= pos && pos <= _size, "eostring::insert");
    
      //    if( _capacity < (_size+len+1)) { // Case where we need to reallocate memory
      //       _size     += len;
      //       _capacity = _size*2;
        
      //       char* begin{expand_mcpy(pos, _capacity, _begin)};
        
      //       memcpy(begin+pos, str, len);
      //       memcpy(begin+len+pos, _begin+pos, _size-len-pos);

      //       delete[] _begin;

      //       _begin        = begin;
      //       _begin[_size] = '\0';
      //    }
      //    else { // Case where we need not reallocate memory
      //       _size += len;
      //       memmove(_begin+pos+len, _begin+pos, _size-pos);
      //       memcpy(_begin+pos, str, len);
      //       _begin[_size] = '\0';
      //    }
    
      //    return *this;
      // }

      // eostring& insert(const size_t pos, const eostring& str) {
      //    eosio::check(0 <= pos && pos <= _size, "eostring::insert");

      //    insert(pos, str.c_str());
    
      //    return *this;
      // }

      // eostring& erase(size_t pos = 0, size_t len = npos) {
      //    eosio::check(0 <= pos && pos <= _size, "eostring::erase");

      //    if(len == eostring::npos)
      //       len = _size-pos;
    
      //    _size -= len;
      //    memmove(_begin+pos+len, _begin, len);
      //    _begin[_size] = '\0';
    
      //    return *this;
      // }

      // eostring& append(const char* str) {
      //    eosio::check(str != nullptr, "eostring::append");
      //    insert(_size, str);
    
      //    return *this;
      // }
    
      // eostring& append(const eostring& str) {
      //    insert(_size, str);
      //    return *this;
      // }

      // eostring& operator+=(const char c) {
      //    if(_capacity == 0) {
      //       _size     = 2;
      //       _capacity = 2*2;
      //       clone(1, _capacity, &c);
      //    }
      //    else if(_size == _capacity) {
      //       std::get<std::unique_ptr<char[]>>(_begin).get()[_size] = c;
      //       _capacity     = ++_size*2;
      //       clone(_size, _capacity, std::get<const char*>(_begin));
      //    }
      //    else {
      //       std::get<std::unique_ptr<char[]>>(_begin).get()[_size]   = c;
      //       std::get<std::unique_ptr<char[]>>(_begin).get()[++_size] = '\0';
      //    }
    
      //    return *this;
      // }

      // eostring& operator+=(const char* rhs) {
      //    for (size_t i{0}; i < strlen(rhs); ++i) {
      //       *this += rhs[i];
      //    }
    
      //    return *this;
      // }

      // eostring& operator+=(const eostring& rhs) {
      //    for (size_t i{0}; i < rhs.size(); ++i) {
      //       *this += rhs[i];
      //    }
    
      //    return *this;
      // }
   
      // friend bool operator< (const eostring& lhs, const eostring& rhs);
      // friend bool operator> (const eostring& lhs, const eostring& rhs);
      // friend bool operator<=(const eostring& lhs, const eostring& rhs);
      // friend bool operator>=(const eostring& lhs, const eostring& rhs);
      // friend bool operator==(const eostring& lhs, const eostring& rhs);
      // friend bool operator!=(const eostring& lhs, const eostring& rhs);
   
      // friend eostring operator+ (const eostring& lhs, const eostring& rhs);

   public:
      size_t _size = 0;
      size_t _capacity = 0;
      std::variant<const char*, std::unique_ptr<char[]>> _begin = nullptr;

      void clone(size_t size, size_t capacity, const char* str) {
         _size     = size;
         _capacity = capacity;
         auto tmp  = std::make_unique<char[]>(capacity);
         memcpy(tmp.get(), str, size);
         _begin = std::move(tmp);
         // begin[size] = '\0'; // This should only be in c_str() don't forget
      }

      char* expand_mset(size_t size, size_t capacity, const char c) {
         char* begin{new char[capacity]};
         memset(begin, c, size);
         begin[size] = '\0';
        
         return begin;
      }
   };

   // bool operator< (const eostring& lhs, const eostring& rhs) {
   //    const char* beg_lhs{lhs._begin}; const char* end_lhs{lhs._begin + lhs._size};
   //    const char* beg_rhs{rhs._begin}; const char* end_rhs{rhs._begin + rhs._size};
   
   //    for(; beg_lhs != end_lhs && beg_rhs != end_rhs; ++beg_lhs, ++beg_rhs) {
   //       if (*beg_lhs < *beg_rhs)
   //          return true;
   //       if (*beg_rhs < *beg_lhs)
   //          return false;
   //    }
   
   //    return beg_lhs == end_lhs && beg_rhs != end_rhs;
   // }

   // bool operator> (const eostring& lhs, const eostring& rhs) {
   //    return (rhs < lhs);
   // }

   // bool operator<=(const eostring& lhs, const eostring& rhs) {
   //    return !(rhs < lhs);
   // }

   // bool operator>=(const eostring& lhs, const eostring& rhs) {
   //    return !(lhs < rhs);
   // }

   // bool operator==(const eostring& lhs, const eostring& rhs) {
   //    return !(lhs < rhs) && !(rhs < lhs);
   // }

   // bool operator!=(const eostring& lhs, const eostring& rhs) {
   //    return !(lhs == rhs);
   // }

   // eostring operator+(const eostring& lhs, const eostring& rhs) {
   //    eostring res{lhs};
   //    res += rhs;
   //    return res;
   // }
   
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
