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
   // assign
   // shrink_to_fit
   // reserve
   // insert
   // erase
   // push_back
   // pop_back
   // append
   // operator+=
   // compare
   // starts_with
   // ends_with
   // replace
   // substr
   // copy
   // resize
   // swap

   friend bool operator+ (const eostring& lhs, const eostring& rhs);
   friend bool operator==(const eostring& lhs, const eostring& rhs);
   friend bool operator!=(const eostring& lhs, const eostring& rhs);
   friend bool operator< (const eostring& lhs, const eostring& rhs);
   friend bool operator> (const eostring& lhs, const eostring& rhs);
   friend bool operator<=(const eostring& lhs, const eostring& rhs);
   friend bool operator>=(const eostring& lhs, const eostring& rhs);

public:
   size_t _size;
   size_t _capacity;
   char*  _begin;
};
