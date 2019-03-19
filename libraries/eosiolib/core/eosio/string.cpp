#include <cstring>

#include "string.hpp"

string::string() : _size{0}, _capacity{0}, _begin{nullptr}
{ }

string::string(const size_t n, const char c) : _size{n}, _capacity{_size*2} {
    _begin = impl::expand_mset(_size, _capacity, c);
}

string::string(const string& str, size_t pos, size_t n) : _size{n}, _capacity{_size*2} {
    if(n ==  string::npos || str._size < pos+n) {
        _size     = str._size;
        _capacity = _size*2;
    }
    
    _begin = impl::expand_mcpy(_size, _capacity, str.data()+pos);
}

string::string(const char* str, const size_t n) : _size{n}, _capacity{_size*2} {
    assert(str != nullptr);
    
    _begin = impl::expand_mcpy(_size, _capacity, str);
}

string::string(const string& str) : _size{str._size}, _capacity{str._capacity} {
    _begin = impl::expand_mcpy(_size, _capacity, str._begin);
}

string::string(string&& str) {
    _size         = str._size;
    _capacity     = str._capacity;
    _begin        = str._begin;
    
    str._size     = 0;
    str._capacity = 0;
    str._begin    = nullptr;
}

string::~string() {
    delete[] _begin;
}

string& string::operator=(const string& str) {
    if(&str == this)
        return *this;
    
    _size     = str._size;
    _capacity = str._capacity;
    _begin    = impl::expand_mcpy(_size, _capacity, str._begin);
    
    return *this;
}

string& string::operator=(string&& s) {
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

string& string::operator=(const char* str) {
    assert(str != nullptr);
    
    _size     = strlen(str);
    _capacity = _size*2;
    _begin    = impl::expand_mcpy(_size, _capacity, str);
    
    return *this;
}

string& string::operator+=(const char c) {
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

string::reference string::at(const size_t n) {
    impl::check(*this, n);
    
    return _begin[n];
}

string::const_reference string::at(const size_t n) const {
    impl::check(*this, n);
    
    return _begin[n];
}

string::reference string::operator[](const size_t n) {
    return _begin[n];
}

string::const_reference string::operator[](const size_t n) const {
    return _begin[n];
}

char& string::front() {
    return _begin[0];
}

const char& string::front() const {
    return _begin[0];
}

char& string::back() {
    return _begin[_size-1];
}

const char& string::back() const {
    return _begin[_size-1];
}

char* string::data() {
    return _begin;
}

const char* string::data() const {
    return _begin;
}

const char* string::c_str() const {
    return _begin;
}

string::iterator string::begin() {
    return string::iterator{&_begin[0]};
}

string::const_iterator string::cbegin() const {
    return string::const_iterator{&_begin[0]};
}

string::iterator string::end() {
    return string::iterator{&_begin[_size]};
}

string::const_iterator string::cend() const {
    return string::const_iterator{&_begin[_size]};
}

bool string::empty() const {
    return !_size;
}

size_t string::size() const {
    return _size;
}

size_t string::length() const {
    return _size;
}

size_t string::capacity() const {
    return _capacity;
}

size_t string::max_size() const {
    return npos;
}

bool operator< (const string& lhs, const string& rhs) {
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

bool operator> (const string& lhs, const string& rhs) {
    return (rhs < lhs);
}

bool operator<=(const string& lhs, const string& rhs) {
    return !(rhs < lhs);
}

bool operator>=(const string& lhs, const string& rhs) {
    return !(lhs < rhs);
}

bool operator==(const string& lhs, const string& rhs) {
    return !(lhs < rhs) && !(rhs < lhs);
}

bool operator!=(const string& lhs, const string& rhs) {
    return !(lhs == rhs);
}

void string::clear() {
    _size     = 0;
    _begin[0] = '\0';
}

void string::reserve(const size_t n) {
    if(_capacity < n)
        _capacity = n;
    else
        return;
}

string& string::insert(size_t pos, const string& s) {
    assert(pos <= _size);
    if( _capacity < (_size + s._size + 1)) {
        size_t orig_sz{_size};
        
        _size       += s._size+1;
        _capacity   = _size*2;
        char* begin = new char[_capacity];
        
        memcpy(begin, _begin, pos);
        memcpy(begin+pos, s._begin, s._size);
        
        orig_sz -= pos;
        
        memcpy(begin+s._size+pos, _begin+pos, orig_sz);
        delete[] _begin;
        
        _begin        = begin;
        _begin[_size] = '\0';
    }
    else {
        _size += s._size+1;
        memmove(_begin+pos+s._size, s._begin, s._size);
    }
    return *this;
}

string& string::operator+=(const string& rhs) {
    for (size_t i{0}; i < rhs.size(); ++i)
        *this += rhs[i];
    return *this;
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

    void check(const string& str, const size_t n) {
        if(n < 0 || str.size() <= n)
            throw "string::at()";
    }
}
