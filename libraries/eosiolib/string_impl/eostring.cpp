// TODO:
// memmove vs memcpy?
// TODO:
// Remove duplicate code
// TODO:
// Organize tests and implementation to be in line with doc
// TODO:
// Asserts
// TODO:
// Make sure memory is getting allocated appropriately
// Consider: explicit, inline, noexcept
// Note:
// that making an empty string and printing `data()` is valid; does not throw. Design this same functionality and test
// Note:
// Look at the standard to see what funcitons in question do.
// Note:
// If an argument given is `eostring::npos` that value is the size of the given string

#include <cstring>   // memcpy, memset, strlen

#include <iostream>  ///
using namespace std; ///

#include "eostring.hpp"

eostring::eostring() : _size{0}, _capacity{0}, _begin{nullptr}
{ }

eostring::eostring(const size_t n, const char c) : _size{n}, _capacity{_size*2} {
    _begin = impl::expand_mset(_size, _capacity, c);
}

eostring::eostring(const eostring& str, const size_t pos, const size_t n) : _size{n}, _capacity{_size*2} {
    if(n ==  eostring::npos || str._size < pos+n) {
        _size     = str._size;
        _capacity = _size*2;
    }
    
    _begin = impl::expand_mcpy(_size, _capacity, str.data()+pos);
}

eostring::eostring(const char* str, const size_t n) : _size{n}, _capacity{_size*2} {
    assert(str != nullptr);
    
    _begin = impl::expand_mcpy(_size, _capacity, str);
}

eostring::eostring(const eostring& str) : _size{str._size}, _capacity{str._capacity} {
    _begin = impl::expand_mcpy(_size, _capacity, str._begin);
}

eostring::eostring(eostring&& str) {
    _size         = str._size;
    _capacity     = str._capacity;
    _begin        = str._begin;
    
    str._size     = 0;
    str._capacity = 0;
    str._begin    = nullptr;
}

eostring::~eostring() {
    delete[] _begin;
}

eostring& eostring::operator=(const eostring& str) {
    if(&str == this)
        return *this;
    
    _size     = str._size;
    _capacity = str._capacity;
    _begin    = impl::expand_mcpy(_size, _capacity, str._begin);
    
    return *this;
}

eostring& eostring::operator=(eostring&& s) {
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

eostring& eostring::operator=(const char* str) {
    assert(str != nullptr);
    
    _size     = strlen(str);
    _capacity = _size*2;
    _begin    = impl::expand_mcpy(_size, _capacity, str);
    
    return *this;
}

eostring::reference eostring::at(const size_t n) {
    impl::check(*this, n);
    
    return _begin[n];
}

eostring::const_reference eostring::at(const size_t n) const {
    impl::check(*this, n);
    
    return _begin[n];
}

eostring::reference eostring::operator[](const size_t n) {
    return _begin[n];
}

eostring::const_reference eostring::operator[](const size_t n) const {
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
    return eostring::iterator{&_begin[0]};
}

eostring::const_iterator eostring::cbegin() const {
    return eostring::const_iterator{&_begin[0]};
}

eostring::iterator eostring::end() {
    return eostring::iterator{&_begin[_size]};
}

eostring::const_iterator eostring::cend() const {
    return eostring::const_iterator{&_begin[_size]};
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

size_t eostring::max_size() const {
    return npos;
}

void eostring::reserve(const size_t n) {
    if(_capacity < n) {
        _capacity = n;
        _begin = impl::expand_mcpy(_size, _capacity, _begin);
    }
    else
        return;
}

size_t eostring::capacity() const {
    return _capacity;
}

void eostring::shrink_to_fit() {
    _capacity = _size;
}

void eostring::clear() {
    _size     = 0;
    _begin[0] = '\0';
}

eostring& eostring::insert(const size_t pos, const char* str) {
    assert(str != nullptr);
    assert(0 <= pos && pos <= _size);

    size_t str_sz{strlen(str)};
    
    if( _capacity < (_size+str_sz+1)) { // Case where we need to reallocate memory
        size_t orig_sz{_size}; // Save the original size of the string for calculations
        
        _size     += str_sz;
        _capacity = _size*2;
        
        char* begin{impl::expand_mcpy(pos, _capacity, _begin)};
        
        memcpy(begin+pos, str, str_sz);
        orig_sz -= pos;
        memcpy(begin+str_sz+pos, _begin+pos, orig_sz);

        delete[] _begin;

        _begin        = begin;
        _begin[_size] = '\0';
    }
    else { // Case where we need not reallocate memory
        _size += str_sz+1;
        memmove(_begin+pos+str_sz, str, str_sz);
    }
    
    return *this;
}

eostring& eostring::insert(const size_t pos, const eostring& str) {
    assert(0 <= pos && pos <= _size);

    this->insert(pos, str.c_str());
    
    return *this;
}

eostring& eostring::erase(size_t pos, size_t len) {
    assert(0 <= pos && pos <= _size);

    if(len == eostring::npos)
        len = _size-pos;
    
    _size -= len;
    memmove(_begin+pos+len, _begin, len);
    _begin[_size] = '\0';
    
    return *this;
}

void eostring::push_back(const char c) {
    *this += c;
}

void eostring::pop_back() {
    if(_size == 0)
        return;
   --_size;
   _begin[_size] = '\0';
}

eostring& eostring::append(const char* str) {
    assert(str != nullptr);
    this->insert(_size, str);
    return *this;
}
    
eostring& eostring::append(const eostring& str) {
    this->insert(_size, str);
    return *this;
}

eostring& eostring::operator+=(const char c) {
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

eostring& eostring::operator+=(const eostring& rhs) {
    for (size_t i{0}; i < rhs.size(); ++i)
        *this += rhs[i];
    return *this;
}

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
