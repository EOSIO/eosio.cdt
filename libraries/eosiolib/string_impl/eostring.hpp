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
   
   eostring();
   eostring(const size_t n, const char c);
   eostring(const eostring& str, const size_t pos, const size_t n = eostring::npos);
   eostring(const char* str, const size_t n);
   eostring(const eostring& str);
   eostring(eostring&& str);
   ~eostring();
    
   eostring& operator=(const eostring& str);
   eostring& operator=(eostring&& str);
   eostring& operator=(const char* str);

   char& at(const size_t n);
   const char at(const size_t n) const;
   char& operator[](const size_t n);
   const char operator[](const size_t n) const;

   char& front();
   const char front() const;
   char& back();
   const char  back() const;
   char* data();
   const char* data()  const;
   const char* c_str() const;

   char* begin();
   const char* cbegin() const;
   char* end();
   const char* cend() const;

   bool empty() const;
   size_t size() const;
   size_t length() const;
   size_t capacity() const;
   size_t max_size() const;
   
   void reserve(const size_t n);
   void shrink_to_fit();
   void clear();
   void resize (const size_t n);
   void swap (eostring& str);
   void push_back(const char c);
   void pop_back();
   eostring substr (const size_t pos = 0, const size_t len = npos) const;
   size_t copy (char* s, const size_t len, const size_t pos = 0) const;

   eostring& insert(const size_t pos, const char* str);
   eostring& insert(const size_t pos, const eostring& str);
   eostring& erase(const size_t pos=0, const size_t len=npos);
   eostring& append(const char* str);
   eostring& append(const eostring& str);
   eostring& operator+=(const char c);
   eostring& operator+=(const char* rhs);
   eostring& operator+=(const eostring& rhs);
   
   friend bool operator< (const eostring& lhs, const eostring& rhs);
   friend bool operator> (const eostring& lhs, const eostring& rhs);
   friend bool operator<=(const eostring& lhs, const eostring& rhs);
   friend bool operator>=(const eostring& lhs, const eostring& rhs);
   friend bool operator==(const eostring& lhs, const eostring& rhs);
   friend bool operator!=(const eostring& lhs, const eostring& rhs);
   
   friend eostring operator+ (const eostring& lhs, const eostring& rhs);

public:
   size_t _size;     // uint32 or less
   size_t _capacity; // uint32 or less
   char*  _begin;
};
