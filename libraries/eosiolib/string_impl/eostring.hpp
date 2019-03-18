static constexpr size_t npos{std::numeric_limits<size_t>::max()};

class eostring
{
public:
    using iterator = char*;
    using const_iterator = const char*;
   
    eostring();
    eostring(const char* s);
    eostring(const char* s, size_t n);
    eostring(size_t n, char c);
    eostring(const eostring& s);
    eostring(eostring&& s);
    ~eostring();
    
    eostring& operator=(const eostring& s);
    eostring& operator=(eostring&& s);
    eostring& operator=(const char* s);
    eostring& operator=(char c);

    eostring& operator+=(char c);

    char& at(size_t n);
    const char& at(size_t n) const;

    char& operator[](size_t n);
    const char& operator[](size_t n) const;
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
    void reserve(size_t n);
    void shrink_to_fit();
    eostring& insert(size_t pos, const eostring& s);
    eostring& erase(size_t pos=0, size_t len=npos);
    void push_back(char c);
    void pop_back();
    eostring& operator+=(const eostring& rhs);
    eostring& replace(size_t pos,  size_t len,  const eostring& s);
    eostring substr (size_t pos = 0, size_t len = npos) const;
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
