/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#pragma once

#include <cstring>   // memcpy, memset, strlen
#include <algorithm> // std::swap
#include <memory>    // std::unique_ptr
#include <variant>   // std::variant
#include <vector>    // std::vector

#include "datastream.hpp" // eosio::datastream
#include "varint.hpp"     // eosio::unsigned_int

namespace eosio {

   class eostring {
   public:
      static constexpr size_t npos = -1;

      template <size_t N>
      constexpr eostring(const char (&str)[N]) : _size{N-1}, _capacity{_size}, _begin{str}
      { }

      constexpr eostring() : _size{0}, _capacity{0}, _begin{""}
      { }

      constexpr eostring(const char* str, const size_t n) : _size{n}, _capacity{_size*2} {
         char* begin{new char[_capacity]};
         memcpy(begin, str, _size);
         _begin = begin;
      }

      constexpr eostring(const size_t n, const char c) : _size{n}, _capacity{_size*2} {
         char* begin{new char[_capacity]};
         memset(begin, c, _size);
         _begin = begin;
      }

      constexpr eostring(const eostring& str, const size_t pos, const size_t n) : _size{n}, _capacity{_size*2} {
         if (n == eostring::npos || str._size < pos+n) {
            _size     = str._size;
            _capacity = _size*2;
         }

         clone(_size, _capacity, str.data()+pos);
      }

      constexpr eostring(const eostring& str) : _size{str._size}, _capacity{str._capacity} {
         if (str.is_sso())
            _begin = std::get<const char*>(str._begin);
         else
            clone(str._size, str._capacity, str.data());
      }

      constexpr eostring(eostring&& str) : _size{str._size}, _capacity{str._capacity} {
         if (str.is_sso())
            _begin = std::get<const char*>(str._begin);
         else
            _begin = std::move(std::get<uptr>(str._begin));
      }

      eostring& operator=(const eostring& str) {
         if (&str == this)
            return *this;

         _size     = str._size;
         _capacity = str._capacity;

         if (str.is_sso())
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

         if (str.is_sso())
            _begin = std::get<const char*>(str._begin);
         else
            _begin = std::move(std::get<uptr>(str._begin));

         return *this;
      }

      eostring& operator=(const char* str) {
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
         return (is_sso()) ? std::get<const char*>(_begin)[n] : std::get<uptr>(_begin).get()[n];
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
         // Why is `str` static? Because without it being static, there would be a memory leak.
         static uptr str{std::make_unique<char[]>(_size+1)};
         const char* tmp{(is_sso()) ? std::get<const char*>(_begin) : std::get<uptr>(_begin).get()};

         memcpy(str.get(), tmp, _size);
         str[_size] = '\0';

         return str.get();
      }

      char* begin() {
         if (is_sso()) {
            // You might be thinking, why aren't we setting `_size` as well?
            // Well, there's no need to set `_size`, because it is already at its proper value.
            _capacity *= 2;
            clone(_size, _capacity, std::get<const char*>(_begin));
         }

         return std::get<uptr>(_begin).get();
      }

      const char* cbegin() const {
         return (is_sso()) ? std::get<const char*>(_begin) : std::get<uptr>(_begin).get();
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

            const char* tmp{(is_sso()) ? std::get<const char*>(_begin) : std::get<uptr>(_begin).get()};
            clone(_size, _capacity, tmp);
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
         if (is_sso())
            clone(n, _capacity, std::get<const char*>(_begin));
         else {
            _size = n;
            if (n <= _capacity)
               memset(std::get<uptr>(_begin).get()+_size, '\0', _capacity-_size);
            else {
               _capacity = _size*2;
               clone(_size, _capacity, std::get<uptr>(_begin).get());
            }
         }
      }

      void swap(eostring& str) {
         std::swap(*this, str);
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

         const char* tmp{(is_sso()) ? std::get<const char*>(_begin) : std::get<uptr>(_begin).get()};
         memcpy(s, tmp+pos, len);

         return (_size < pos+len) ? _size-pos : len;
      }

      eostring& insert(const size_t pos, const char* str) {
         return insert(pos, str, strlen(str));
      }

      eostring& insert(const size_t pos, const char* str, const size_t len) {
         eosio::check((str != nullptr) && (0 <= pos && pos <= _size), "eostring::insert");

         // If `insert` causes the string to exceed its `_capacity`, a new string will have to be constructed.
         // Else, the next branch will be taken; determining if a new string has to be constructed.
         if (_capacity < (_size+len)) {
            _size      += len;
            _capacity  = _size*2;

            uptr begin{std::make_unique<char[]>(_capacity)};
            memcpy(begin.get(), std::get<const char*>(_begin), pos);
            memcpy(begin.get()+pos, str, len);
            memcpy(begin.get()+len+pos, std::get<const char*>(_begin)+pos, _size-len-pos);

            _begin = std::move(begin);
         }
         else {
            if(is_sso())
               clone(_size, _capacity, std::get<const char*>(_begin));
            _size += len;
            memmove(std::get<uptr>(_begin).get()+pos+len, std::get<uptr>(_begin).get()+pos, _size-pos);
            memcpy(std::get<uptr>(_begin).get()+pos, str, len);
         }

         return *this;
      }

      eostring& insert(const size_t pos, const eostring& str) {
         insert(pos, str.c_str());
         return *this;
      }

      eostring& erase(size_t pos = 0, size_t len = npos) {
         eosio::check(0 <= pos && pos <= _size, "eostring::erase");

         if (len == eostring::npos)
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

            const char* tmp{(is_sso()) ? std::get<const char*>(_begin) : std::get<uptr>(_begin).get()};
            clone(_size, _capacity, tmp);
            std::get<uptr>(_begin).get()[_size-1] = c;
         }
         else
            std::get<uptr>(_begin).get()[_size++] = c;

         return *this;
      }

      eostring& operator+=(const char* rhs) {
         append(rhs);
         return *this;
      }

      eostring& operator+=(const eostring& rhs) {
         append(rhs);
         return *this;
      }

      inline void print() const {
         const char* tmp{(is_sso()) ? std::get<const char*>(_begin) : std::get<uptr>(_begin).get()};
         internal_use_do_not_use::prints_l(tmp, _size);
      }

      friend bool operator< (const eostring& lhs, const eostring& rhs);
      friend bool operator> (const eostring& lhs, const eostring& rhs);
      friend bool operator<=(const eostring& lhs, const eostring& rhs);
      friend bool operator>=(const eostring& lhs, const eostring& rhs);
      friend bool operator==(const eostring& lhs, const eostring& rhs);
      friend bool operator!=(const eostring& lhs, const eostring& rhs);

      friend eostring operator+ (const eostring& lhs, const eostring& rhs);

   private:
      using uptr    = std::unique_ptr<char[]>;
      using sso_str = std::variant<const char*, uptr>;

      size_t  _size     = 0;
      size_t  _capacity = 0;
      sso_str _begin    = nullptr;

      void clone(size_t size, size_t capacity, const char* str) {
         _size     = size;
         _capacity = capacity;

         uptr begin{std::make_unique<char[]>(capacity)};
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

   template<typename DataStream>
   DataStream& operator<<(DataStream& ds, const eostring& str) {
      ds << unsigned_int(str.size());
      if (str.size())
         ds.write(str.data(), str.size());
      return ds;
   }

   template<typename DataStream>
   DataStream& operator>>(DataStream& ds, eostring& str) {
      std::vector<char> tmp;
      ds >> tmp;
      str = (tmp.size()) ? eostring(tmp.data(), tmp.size()) : eostring();
      return ds;
   }

} // namespace eosio
