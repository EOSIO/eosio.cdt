class eostring;

namespace impl {
    char* expand_mcpy(const size_t size, const size_t capacity, const char* str);
    char* expand_mset(const size_t size, const size_t capacity, const char c);
    void check(const eostring& str, const size_t n);
}

class eostring
{
public:
    static constexpr size_t npos{std::numeric_limits<size_t>::max()};
    
    using iterator = char*;
    using const_iterator = const char*;
    using reference = char&;
    using const_reference = const char&;

    template <size_t N> 
    constexpr eostring(const char (&str)[N]) : _size{N-1}, _capacity{_size*2} {
        _begin = impl::expand_mcpy(_size, _capacity, str);
    }
   
    eostring();
    eostring(const char* s, const size_t n);
    eostring(const size_t n, const char c);
    eostring(const eostring& str, const size_t pos, const size_t n = eostring::npos);
    eostring(const eostring& s);
    eostring(eostring&& s);
    ~eostring();
    
    eostring& operator=(const eostring& s);
    eostring& operator=(eostring&& s);
    eostring& operator=(const char* s);
    eostring& operator=(const char c);

    eostring& operator+=(const char c);

    reference at(const size_t n);
    const_reference at(const size_t n) const;
    reference operator[](const size_t n);
    const_reference operator[](const size_t n) const;
    
    char& front();
    const char& front() const;
    char& back();
    const char& back() const;
    char* data();
    const char* data() const;
    const char* c_str() const;

    iterator begin();
    const_iterator cbegin() const;
    iterator end();
    const_iterator cend() const;

    bool empty() const;
    size_t size() const;
    size_t length() const;
    size_t max_size() const;
    size_t capacity() const;
    void clear();
    void reserve(const size_t n);
    void shrink_to_fit();
    eostring& insert(const size_t pos, const eostring& s);
    eostring& erase(const size_t pos=0, const size_t len=npos);
    void push_back(const char c);
    void pop_back();
    eostring& operator+=(const eostring& rhs);
    eostring& replace(const size_t pos,  const size_t len,  const eostring& s);
    eostring substr (const size_t pos = 0, const size_t len = npos) const;
    size_t copy (char* s, size_t len, size_t pos = 0) const;
    // void resize (size_t n);
    // void swap (eostring& str);

    friend bool operator+ (const eostring& lhs, const eostring& rhs);
    friend bool operator< (const eostring& lhs, const eostring& rhs);
    friend bool operator> (const eostring& lhs, const eostring& rhs);
    friend bool operator<=(const eostring& lhs, const eostring& rhs);
    friend bool operator>=(const eostring& lhs, const eostring& rhs);
    friend bool operator==(const eostring& lhs, const eostring& rhs);
    friend bool operator!=(const eostring& lhs, const eostring& rhs);

public:
    size_t _size;     // uint32 or less
    size_t _capacity; // uint32 or less
    char*  _begin;
};
