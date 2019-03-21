#pragma once

#include <cassert> // assert
#include <cstring> // memcpy, memset, strlen
#include <utility> // std::move
#include <vector>  // std::vector

// #include "datastream.hpp" // eosio::datastream
#include "eostring.hpp"
// #include "varint.hpp" // eosio::unsigned_int

namespace eosio {

   class eostring;
   
   namespace impl {
      char* expand_mcpy(const size_t size, const size_t capacity, const char* str);
      char* expand_mset(const size_t size, const size_t capacity, const char c);
      void check(const eostring& str, const size_t n);
   }

   class eostring
   {
   public:
      static constexpr size_t npos = -1;

      template <size_t N> 
      constexpr eostring(const char (&str)[N]) : _size{N-1}, _capacity{_size*2} {
         _begin = impl::expand_mcpy(_size, _capacity, str);
      }
   
      eostring() : _size{0}, _capacity{0}, _begin{nullptr}
      { }
      
      eostring(const size_t n, const char c) : _size{n}, _capacity{_size*2} {
         _begin = impl::expand_mset(_size, _capacity, c);
      }
      
      eostring(const eostring& str, const size_t pos, const size_t n) : _size{n}, _capacity{_size*2} {
         if(n ==  eostring::npos || str._size < pos+n) {
            _size     = str._size;
            _capacity = _size*2;
         }
    
         _begin = impl::expand_mcpy(_size, _capacity, str.data()+pos);
      }
      
      eostring(const char* str, const size_t n) : _size{n}, _capacity{_size*2} {
         assert(str != nullptr);
    
         _begin = impl::expand_mcpy(_size, _capacity, str);
      }
      
      eostring(const eostring& str) : _size{str._size}, _capacity{str._capacity} {
         _begin = impl::expand_mcpy(_size, _capacity, str._begin);
      }
      
      eostring(eostring&& str) {
         _size         = str._size;
         _capacity     = str._capacity;
         _begin        = str._begin;
    
         str._size     = 0;
         str._capacity = 0;
         str._begin    = nullptr;
      }
      
      ~eostring() {
         delete[] _begin;
      }
    
      eostring& operator=(const eostring& str) {
         if(&str == this)
            return *this;
    
         _size     = str._size;
         _capacity = str._capacity;
         _begin    = impl::expand_mcpy(_size, _capacity, str._begin);
    
         return *this;
      }

      eostring& operator=(eostring&& s) {
         if(&s == this)
            return *this;
    
         _size       = s._size;
         _capacity   = s._capacity;
         _begin      = s._begin;
    
         s._size     = 0;
         s._capacity = 0;
         s._begin    = nullptr;
    
         return *this;
      }

      eostring& operator=(const char* str) {
         assert(str != nullptr);
    
         _size     = strlen(str);
         _capacity = _size*2;
         _begin    = impl::expand_mcpy(_size, _capacity, str);
    
         return *this;
      }

      char& at(const size_t n) {
         impl::check(*this, n);
    
         return _begin[n];
      }

      const char at(const size_t n) const {
         impl::check(*this, n);
    
         return _begin[n];
      }

      char& operator[](const size_t n) {
         return _begin[n];
      }

      const char operator[](const size_t n) const {
         return _begin[n];
      }

      char& front() {
         return _begin[0];
      }

      const char front() const {
         return _begin[0];
      }

      char& back() {
         return _begin[_size-1];
      }

      const char back() const {
         return _begin[_size-1];
      }

      char* data() {
         return _begin;
      }

      const char* data() const {
         return _begin;
      }

      const char* c_str() const {
         return _begin;
      }

      char* begin() {
         char* begin{&_begin[0]};
         return begin;
      }

      const char* cbegin() const {
         const char* begin{&_begin[0]};
         return begin;
      }

      char* end() {
         char* end{&_begin[_size]};
         return end;
      }

      const char* cend() const {
         const char* end{&_begin[_size]};
         return end;
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
            _begin = impl::expand_mcpy(_size, _capacity, _begin);
         }
         else
            return;
      }

      void shrink_to_fit() {
         _capacity = _size;
      }

      void clear() {
         _size     = 0;
         _begin[0] = '\0';
      }

      void resize(const size_t n) {
         if(_capacity < n) {
            size_t old_sz{_size};
        
            _size     = n;
            _capacity = _size*2;
            _begin    = impl::expand_mcpy(old_sz, _capacity, _begin);
         }
         else {
            memset(_begin+n, '\0', _size);
            _size = n;
         }
      }

      void swap(eostring& str) {
         eostring temp = std::move(*this);
    
         *this = std::move(str);
         str   = std::move(temp);
      }

      void push_back(const char c) {
         *this += c;
      }

      void pop_back() {
         if(_size == 0)
            return;
         --_size;
         _begin[_size] = '\0';
      }

      eostring substr(size_t pos = 0, size_t len = npos) const {
         return eostring(*this, pos, len);
      }

      size_t copy(char* s, size_t len, size_t pos = 0) const {
         memcpy(s, substr(pos, len)._begin, substr(pos, len)._size);
         return (_size < pos+len) ? _size-pos : len;
      }

      eostring& insert(const size_t pos, const char* str) {
         assert(str != nullptr);
         assert(0 <= pos && pos <= _size);

         size_t str_sz{strlen(str)};
    
         if( _capacity < (_size+str_sz+1)) { // Case where we need to reallocate memory
            _size     += str_sz;
            _capacity = _size*2;
        
            char* begin{impl::expand_mcpy(pos, _capacity, _begin)};
        
            memcpy(begin+pos, str, str_sz);
            memcpy(begin+str_sz+pos, _begin+pos, _size-str_sz-pos);

            delete[] _begin;

            _begin        = begin;
            _begin[_size] = '\0';
         }
         else { // Case where we need not reallocate memory
            _size += str_sz;
            memmove(_begin+pos+str_sz, _begin+pos, _size-pos);
            memcpy(_begin+pos, str, str_sz);
            _begin[_size] = '\0';
         }
    
         return *this;
      }

      eostring& insert(const size_t pos, const eostring& str) {
         assert(0 <= pos && pos <= _size);

         this->insert(pos, str.c_str());
    
         return *this;
      }

      eostring& erase(size_t pos = 0, size_t len = npos) {
         assert(0 <= pos && pos <= _size);

         if(len == eostring::npos)
            len = _size-pos;
    
         _size -= len;
         memmove(_begin+pos+len, _begin, len);
         _begin[_size] = '\0';
    
         return *this;
      }

      eostring& append(const char* str) {
         assert(str != nullptr);
         this->insert(_size, str);
    
         return *this;
      }
    
      eostring& append(const eostring& str) {
         this->insert(_size, str);
         return *this;
      }

      eostring& operator+=(const char c) {
         if(_capacity == 0) {
            _size     = 2;
            _capacity = 2*2;
            _begin    = impl::expand_mcpy(1, _capacity, &c);
         }
         else if(_size == _capacity) {
            _begin[_size] = c;
            _capacity     = ++_size*2;
            _begin        = impl::expand_mcpy(_size, _capacity, this->_begin);
         }
         else {
            _begin[_size]   = c;
            _begin[++_size] = '\0';
         }
    
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
      size_t _size;
      size_t _capacity;
      char*  _begin;
   };

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

   eostring operator+(const eostring& lhs, const eostring& rhs) {
      eostring res{lhs};
      res += rhs;
      return res;
   }

   namespace impl {
      char* expand_mcpy(size_t size, size_t capacity, const char* str) {
         char* begin{new char[capacity]};
         memcpy(begin, str, size);
         begin[size] = '\0';
        
         return begin;
      }

      char* expand_mset(size_t size, size_t capacity, const char c) {
         char* begin{new char[capacity]};
         memset(begin, c, size);
         begin[size] = '\0';
        
         return begin;
      }

      void check(const eostring& str, const size_t n) {
         if(n < 0 || str.size() <= n)
            throw "eostring::at()";
      }
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
   //    std::vector<char> vec;
   //    ds >> vec;
   //    if(vec.size())
   //       str = eostring(vec.data(),vec.data()+vec.size());
   //    else
   //       str = eostring();
   //    return ds;
   // }
   
} // namespace eosio
