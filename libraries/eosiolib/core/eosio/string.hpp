class string;

namespace impl {
    char* expand_mcpy(const size_t size, const size_t capacity, const char* str);
    char* expand_mset(const size_t size, const size_t capacity, const char c);
    void check(const string& str, const size_t n);
}

class string
{
public:
    static constexpr size_t npos = -1;
    
    using iterator = char*;
    using const_iterator = const char*;
    using reference = char&;
    using const_reference = const char&;

    template <size_t N> 
    constexpr string(const char (&str)[N]) : _size{N-1}, _capacity{_size*2} {
        _begin = impl::expand_mcpy(_size, _capacity, str);
    }
   
    string();
    string(const char* s, const size_t n);
    string(const size_t n, const char c);
    string(const string& str, const size_t pos, const size_t n = string::npos);
    string(const string& s);
    string(string&& s);
    ~string();
    
    string& operator=(const string& s);
    string& operator=(string&& s);
    string& operator=(const char* s);
    string& operator=(const char c);

    string& operator+=(const char c);

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
    string& insert(const size_t pos, const string& s);
    string& erase(const size_t pos=0, const size_t len=npos);
    void push_back(const char c);
    void pop_back();
    string& operator+=(const string& rhs);
    string& replace(const size_t pos,  const size_t len,  const string& s);
    string substr (const size_t pos = 0, const size_t len = npos) const;
    size_t copy (char* s, size_t len, size_t pos = 0) const;

    friend bool operator+ (const string& lhs, const string& rhs);
    friend bool operator< (const string& lhs, const string& rhs);
    friend bool operator> (const string& lhs, const string& rhs);
    friend bool operator<=(const string& lhs, const string& rhs);
    friend bool operator>=(const string& lhs, const string& rhs);
    friend bool operator==(const string& lhs, const string& rhs);
    friend bool operator!=(const string& lhs, const string& rhs);

public:
    size_t _size;
    size_t _capacity;
    char*  _begin;
};
