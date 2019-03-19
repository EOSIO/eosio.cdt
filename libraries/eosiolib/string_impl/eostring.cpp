// TODO:
// Organize tests and implementation to be in line with doc
// TODO:
// Asserts
// Consider: explicit, inline, noexcept
// Note:
// that making an empty string and printing `data()` is valid; does not throw. Design this same functionality and test
// Note:
// Look at the standard to see what funcitons in question do.
// Note:
// If an argument given is `eostring::npos` that value is the size of the given string

#include <cstring>   // memcpy, memset, strlen, strcpy     ???

#include <iostream>  ///
using namespace std; ///

#include "eostring.hpp"

eostring::eostring() : _size{0}, _capacity{0}, _begin{nullptr}
{ }

eostring::eostring(const size_t n, const char c) : _size{n}, _capacity{_size*2} {
    _begin = impl::expand_mset(_size, _capacity, c);
}

eostring::eostring(const eostring& str, size_t pos, size_t n) : _size{n}, _capacity{_size*2} {
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

// eostring::reverse_iterator eostring::rbegin() {
//     return eostring::iterator{&_begin[_size]-1};
// }

// eostring::const_reverse_iterator eostring::crbegin() const {
//     return eostring::const_reverse_iterator{&_begin[_size]-1};
// }

// eostring::reverse_iterator eostring::rend() {
//     return eostring::reverse_iterator{&_begin[0]-1};
// }

// eostring::const_reverse_iterator eostring::crend() const {
//     return eostring::const_reverse_iterator{&_begin[0]-1};
// }

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
    _size     = 0;
    _begin[0] = '\0';
}

void eostring::reserve(const size_t n) {
    if(_capacity < n)
        _capacity = n;
    else
        return;
}

eostring& eostring::insert(size_t pos, const eostring& s) {
    assert(pos <= _size);
    // if( _capacity == 0) { // Don't need
    //    assert(pos == 0);
    //    _size = s._size;
    //    _capacity = _size*2;
    //    memcpy(_begin, s._begin, s._size);
    //    _begin[_size] = '\n';
    // }
    if( _capacity < (_size + s._size + 1)) {
        size_t orig_sz{_size};
        _size += s._size+1;
        _capacity = _size*2;
        char* begin = new char[_capacity];
        memcpy(begin, _begin, pos);
        memcpy(begin+pos, s._begin, s._size);
        orig_sz -= pos;
        memcpy(begin+s._size+pos, _begin+pos, orig_sz);
        delete[] _begin;
        _begin = begin;
        _begin[_size] = '\0';
    }
    else {
        // size_t orig_sz{_size};
        // _size += s._size+1;
        // char* begin = new char[_capacity];
        // memcpy(begin, _begin, pos);
        // memcpy(begin+pos, s._begin, s._size);
        // orig_sz -= pos;
        // if(orig_sz)
        //    memcpy(begin+s._size+pos, _begin+pos, orig_sz);
        // delete[] _begin;
        // _begin = begin;
        // _begin[_size] = '\0';
        _size += s._size+1;
        memmove(_begin+pos+s._size, s._begin, s._size);
    }
    return *this;
}

// eostring& eostring::erase(size_t pos, size_t len) { // Look over this again
//    assert(pos < )
//    cout << len;
//    _size -= len;
//    _capacity = _size;
//    char* begin = new char[_size];
//    memcpy(begin, _begin, pos);
//    memcpy(begin+pos, _begin+pos+len, _size-len);
//    delete[] _begin;
//    _begin = begin;
//    _begin[_size] = '\0';
//    return *this;
// }

void eostring::push_back(char c) {
    *this += c;
}

// void eostring::pop_back() {
//    // for (int i = 0; i < 10; ++i) cout << _begin[i] << ' '; cout << endl;
//    --_size;
//    _capacity = _size;
//    erase(_size, 0);
//    _begin[_size] = '\0';
// }

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
