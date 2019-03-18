#include <iostream>
#include "eostring.hpp"
using namespace std;
int main()
{  
    //// eostring()
    assert(eostring{}.size()     == 0);
    assert(eostring{}.capacity() == 0);
    assert(eostring{}.data()     == nullptr);  

    // //// eostring(const char* s)
    assert(eostring{"a"}.size()              == 1);
    assert(eostring{"a"}.capacity()          == 2);
    assert(strcmp(eostring{"a"}.data(), "a") == 0);
   
    assert(eostring{"abc123"}.size()                   == 6);
    assert(eostring{"abc123"}.capacity()               == 12);
    assert(strcmp(eostring{"abc123"}.data(), "abc123") == 0);

    // //// eostring(const char* s, size_t n)
    eostring eostr0("a", 0);
    eostring eostr1("a", 1);
    eostring eostr2("abcdef", 3);
   
    assert(eostr0.size()     == 0);
    assert(eostr0.capacity() == 0);
    // assert(eostr0.data()     == nullptr); // Need eyes here

    assert(eostr1.size()              == 1);
    assert(eostr1.capacity()          == 2);
    assert(strcmp(eostr1.data(), "a") == 0);

    assert(eostr2.size()                == 3);
    assert(eostr2.capacity()            == 6);
    assert(strcmp(eostr2.data(), "abc") == 0);

    // assert(eostring("abcdef", 7).size()     == 0); // Edge-case; should throw
    // assert(eostring("abcdef", 7).capacity() == 0); // Edge-case; should throw
    // assert(eostring("abcdef", 7).data()     == 0); // Edge-case; should throw

    //// eostring(size_t n, char c)
    assert(eostring(0, 'c').size()     == 0);
    assert(eostring(0, 'c').capacity() == 0);
    // assert(eostring(0, 'c').data()     == nullptr);  // Need eyes here

    assert(eostring(1, 'c').size()              == 1);
    assert(eostring(1, 'c').capacity()          == 2);
    assert(strcmp(eostring(1, 'c').data(), "c") == 0);

    assert(eostring(3, 'c').size()                == 3);
    assert(eostring(3, 'c').capacity()            == 6);
    assert(strcmp(eostring(3, 'c').data(), "ccc") == 0);

    //// eostring(const eostring& s)
    {
        eostring eostr{"abcdef"};
        eostring eostr_cpy(eostr);
   
        assert(eostr_cpy.size() == eostr.size() && (eostr_cpy.size() == 6));
        assert(eostr_cpy.capacity() == eostr.capacity() && eostr_cpy.capacity() == 12);
        assert(strcmp(eostr_cpy.data(), "abcdef") == 0);
        assert(eostr.data() != eostr_cpy.data()); // Need eyes here
    }

    //// eostring(eostring&& s)
    {
        eostring eostr{"abcdef"};
        eostring eostr_mv(std::move(eostr));

        assert(eostr_mv.size()     == 6);
        assert(eostr.size()        == 0);
        assert(eostr_mv.capacity() == 12);
        assert(eostr.size()        == 0);
        assert(strcmp(eostr_mv.data(), "abcdef") == 0);
        assert(eostr.data() != eostr_mv.data()); // Need eyes here
    }

    //// eostring& operator=(const eostring& s);
    {
        eostring eostr{"abcdef"};
        eostring eostr_assig = eostr;

        assert(eostr_assig.size() == eostr.size() && (eostr_assig.size() == 6));
        assert(eostr_assig.capacity() == eostr.capacity() && eostr_assig.capacity() == 12);
        assert(strcmp(eostr_assig.data(), "abcdef") == 0);
        assert(eostr.data() != eostr_assig.data()); // Need eyes here

        eostr_assig = eostr_assig;
        assert(eostr_assig.size() == eostr.size() && (eostr_assig.size() == 6));
        assert(eostr_assig.capacity() == eostr.capacity() && eostr_assig.capacity() == 12);
        assert(strcmp(eostr_assig.data(), "abcdef") == 0);
        assert(eostr.data() != eostr_assig.data()); // Need eyes here
    }
   
    //// eostring& operator=(eostring&& s);
    {
        eostring eostr{"abcdef"};
        eostring eostr_mv_assig = std::move(eostr);

        assert(eostr_mv_assig.size()     == 6);
        assert(eostr.size()              == 0);
        assert(eostr_mv_assig.capacity() == 12);
        assert(eostr.size()              == 0);
        assert(strcmp(eostr_mv_assig.data(), "abcdef") == 0);
        assert(eostr.data() != eostr_mv_assig.data()); // Need eyes here
    }
   
    //// eostring& operator=(const char* s);
    {
        eostring eostr{};
        eostr = "abcdef";
   
        assert(eostr.size()                   == 6);
        assert(eostr.capacity()               == 12);
        assert(strcmp(eostr.data(), "abcdef") == 0);

        eostr = eostr;
        assert(eostr.size()                   == 6);
        assert(eostr.capacity()               == 12);
        assert(strcmp(eostr.data(), "abcdef") == 0);
    }

    //// eostring& operator+=(char c)
    {
        eostring eostr{"abcdef"};
        char c{'g'};
        eostr += c;

        assert(eostr.size()                    == 7);
        assert(eostr.capacity()                == 12);
        assert(strcmp(eostr.data(), "abcdefg") == 0);
    }

    //// char& at(size_t n)
    {
        eostring eostr{"abcdef"};
        assert(eostr.at(0) == 'a');
        assert(eostr.at(5) == 'f');

        // assert(eostr.at(6) == 0); // Edge-case; should throw
    }
      
    //// const char& at(size_t n) const
    {
        const eostring eostr{"abcdef"};
        assert(eostr.at(0) == 'a');
        assert(eostr.at(5) == 'f');

        // assert(eostr.at(6) == 0); // Edge-case; should throw
    }

    //// char& operator[](size_t n)
    {
        eostring eostr{"abcdef"};
        assert(eostr[0] == 'a');
        assert(eostr[5] == 'f');
    }
      
    //// const char& operator[](size_t n) const
    {
        const eostring eostr{"abcdef"};
        assert(eostr[0] == 'a');
        assert(eostr[5] == 'f');
    }
   
    //// char& front()
    {
        eostring eostr{"abcdef"};
        assert(eostr.front() == 'a');
    }
      
    //// const char& front() const
    {
        const eostring eostr{"abcdef"};
        assert(eostr.front() == 'a');
    }
      
    //// char& back()
    {
        eostring eostr{"abcdef"};
        assert(eostr.back() == 'f');
    }
      
    //// const char& back() const
    {
        const eostring eostr{"abcdef"};
        assert(eostr.back() == 'f');
    }
      
    //// char* data()
    {
        eostring eostr{"abcdef"};
        assert(strcmp(eostr.data(), "abcdef") == 0);

        eostr = "abc";
        assert(strcmp(eostr.data(), "abc") == 0);
    }
      
    //// const char* data() const
    {
        const eostring eostr{"abcdef"};
        assert(strcmp(eostr.data(), "abcdef") == 0);
    }
      
    //// const char* c_str() const
    {
        const eostring eostr{"abcdef"};
        assert(strcmp(eostr.c_str(), "abcdef") == 0);
    }

    //// iterator begin()
    {
        eostring eostr{"abcdef"};
        eostring::iterator iter{eostr.begin()};
        assert(iter   == &eostr[0]);
        assert(++iter == &eostr[1]);
    }
   
    //// const_iterator cbegin() const
    {
        const eostring eostr{"abcdef"};
        eostring::const_iterator iter{eostr.cbegin()};
        assert(iter   == &eostr[0]);
        assert(++iter == &eostr[1]);
    }
   
    //// iterator end()
    {
        eostring eostr{"abcdef"};
        eostring::iterator iter{eostr.end()};
        assert(iter   == &eostr[eostr.size()]);
        assert(--iter == &eostr[eostr.size()-1]);
    }
   
    //// const_iterator cend() const
    {
        const eostring eostr{"abcdef"};
        eostring::const_iterator iter{eostr.cend()};
        assert(iter   == &eostr[eostr.size()]);
        assert(--iter == &eostr[eostr.size()-1]);
    }

    //// bool eostring::empty() const
    eostring eostr{};
    assert(eostr.empty() == true);
    eostr += 'c';
    assert(eostr.empty() == false);

    //// size_t eostring::size() const
    {
        eostring eostr{"abcdef"};
        assert(eostr.size() ==  6);
        eostr += 'g';
        assert(eostr.size() ==  7);
    }

    //// size_t eostring::length() const
    {
        eostring eostr{"abcdef"};
        assert(eostr.length() ==  6);
        eostr += 'g';
        assert(eostr.length() ==  7);
    }

    //// size_t eostring::capacity() const
    {
        eostring eostr{"abc"};
        assert(eostr.capacity() ==  6);
        eostr += 'd', eostr += 'e', eostr += 'f';
        assert(eostr.capacity() ==  6);
        eostr += 'g';
        assert(eostr.capacity() ==  14);
    }

    //// size_t eostring::max_size() const
    {
        eostring eostr{"abcdef"};
        assert(eostr.max_size() ==  npos);
    }
   
    //// friend bool operator==(const eostring& lhs, const eostring& rhs)
    {
        eostring eostr0{"abc"};
        eostring eostr1{"def"};
        assert((eostr0 == eostr0) == true);
        assert((eostr1 == eostr1) == true);
        assert((eostr0 == eostr1) == false);
    }
   
    //// friend bool operator!=(const eostring& lhs, const eostring& rhs)
    {
        eostring eostr0{"abc"};
        eostring eostr1{"def"};
        assert((eostr0 != eostr0) == false);
        assert((eostr1 != eostr1) == false);
        assert((eostr0 != eostr1) == true);
    }
   
    //// friend bool operator< (const eostring& lhs, const eostring& rhs) // Make sure to double check this logic
    {
        eostring eostr0{"abc"};
        eostring eostr1{"def"};
        assert((eostr0 < eostr0) == false);
        assert((eostr1 < eostr1) == false);
        assert((eostr0 < eostr1) == true);
    }
   
    //// friend bool operator> (const eostring& lhs, const eostring& rhs)
    {
        eostring eostr0{"abc"};
        eostring eostr1{"def"};
        assert((eostr0 > eostr0) == false);
        assert((eostr1 > eostr1) == false);
        assert((eostr0 > eostr1) == false);
    }
   
    //// friend bool operator<=(const eostring& lhs, const eostring& rhs)
    {
        eostring eostr0{"abc"};
        eostring eostr1{"def"};
        assert((eostr0 <= eostr0) == true);
        assert((eostr1 <= eostr1) == true);
        assert((eostr0 <= eostr1) == true);
    }
   
    //// friend bool operator>=(const eostring& lhs, const eostring& rhs)
    {
        eostring eostr0{"abc"};
        eostring eostr1{"def"};
        assert((eostr0 >= eostr0) == true);
        assert((eostr1 >= eostr1) == true);
        assert((eostr0 >= eostr1) == false);
    }

    //// void eostring::clear()
    {
        eostring eostr{"abcdef"};
        assert(eostr.empty() == false);
        eostr.clear();
        assert(eostr.empty() == true);
        assert(eostr.size()  == 0);
    }

    //// void reserve(size_t n)
    {
        eostring eostr{"abcdef"};
        assert(eostr.capacity() == 12);
        eostr.reserve(10);
        assert(eostr.capacity() == 12);
        eostr.reserve(24);
        assert(eostr.capacity() == 24);
    }

    //// eostring& insert(size_t pos, const eostring& s)
    {
        eostring eostr{"iii"};
        eostring eostr_ins{"ooo"};
        eostr.insert(0, eostr_ins);
        assert(strcmp(eostr.data(), "oooiii") == 0);
    }

    {
        eostring eostr{"iii"};
        eostring eostr_ins{"ooo"};
        eostr.insert(1, eostr_ins);
        assert(strcmp(eostr.data(), "ioooii") == 0);
    }
   
    {
        eostring eostr{"iii"};
        eostring eostr_ins{"ooo"};
        eostr.insert(2, eostr_ins);
        assert(strcmp(eostr.data(), "iioooi") == 0);
    }

    {
        eostring eostr{"iii"};
        eostring eostr_ins{"ooo"};
        eostr.insert(3, eostr_ins);
        assert(strcmp(eostr.data(), "iiiooo") == 0);
    }

    {
        eostring eostr{"abcdefg"};
        eostring eostr_ins{"xxx"};
        eostr.insert(3, eostr_ins);
        // assert(strcmp(eostr.data(), "abcxxxdefg") == 0);
    }

    // { // Should throw out of range exception
    // eostring eostr{"iii"};
    // eostring eostr_ins{"ooo"};
    // eostr.insert(4, eostr_ins);
    // assert(strcmp(eostr.data(), "iiiooo") == 0);
    // }

    // //// eostring& erase(size_t pos = 0, size_t len = npos)
    // {
    // eostring eostr{"abcdefgh"};
    // eostr.erase();
    // assert(strcmp(eostr.data(), "") == 0);
    // }

    // {
    // eostring eostr{"abcdefgh"};
    // eostr.erase(0);
    // assert(strcmp(eostr.data(), "") == 0);
    // }

    // {
    // eostring eostr{"abcdefgh"};
    // eostr.erase(0, npos);
    // assert(strcmp(eostr.data(), "") == 0);
    // eostr.erase(1, npos);
    // assert(strcmp(eostr.data(), "a") == 0);
    // eostr.erase(2, npos);
    // assert(strcmp(eostr.data(), "ab") == 0);
    // eostr.erase(3, npos);
    // assert(strcmp(eostr.data(), "abc") == 0);
    // eostr.erase(4, npos);
    // assert(strcmp(eostr.data(), "abcd") == 0);
    // eostr.erase(5, npos);
    // assert(strcmp(eostr.data(), "abcde") == 0);
    // eostr.erase(6, npos);
    // assert(strcmp(eostr.data(), "abcdef") == 0);
    // eostr.erase(7, npos);
    // assert(strcmp(eostr.data(), "abcdefg") == 0);
    // eostr.erase(8, npos);
    // assert(strcmp(eostr.data(), "abcdefgh") == 0);
    // }

    // {
    // eostring eostr{"abcdefgh"};
    // eostr.erase(8, 0);
    // assert(strcmp(eostr.data(), "abcdefgh") == 0);
    // eostr.erase(8, 1);
    // assert(strcmp(eostr.data(), "abcdefgh") == 0);
    // }

    //// void push_back(char c)
    {
        eostring eostr{"abcdef"};
        assert(eostr.size() == 6);
        assert(eostr.size() == 6);
        eostr.push_back('g');
        assert(eostr.size() == 7);
        assert(eostr.size() == 7);
        assert(strcmp(eostr.data(), "abcdefg") == 0);
    }

    // //// void pop_back()
    // {
    // eostring eostr{"abcdefg"};
    // assert(eostr.size() == 7);
    // assert(eostr.size() == 7);
    // eostr.pop_back();
    // assert(eostr.size() == 6);
    // assert(eostr.size() == 6);
    // assert(strcmp(eostr.data(), "abcdef") == 0);
    // }

    // //// eostring& operator+=(const eostring& s)
    // {
    // eostring eostr{"a"};
    // assert(eostr.size() == 1);
    // eostr += "b";
    // assert(eostr.size() == 2);
    // assert(strcmp(eostr.data(), "ab") == 0);
    // }
   
    // {
    // eostring eostr{"abc"};
    // assert(eostr.size() == 3);
    // eostr += "def";
    // assert(eostr.size() == 6);
    // assert(strcmp(eostr.data(), "abcdef") == 0);
    // }

    // //// eostring& replace (size_t pos,  size_t len,  const eostring& s)
    // {
    // eostring eostr{"abcdef"};
    // eostring eostr_repl{"xx"};
    // eostr.replace(2,2,eostr_repl);
    // assert(strcmp(eostr.data(), "abxxef") == 0);
    // }

    //// eostring substr (size_t pos = 0, size_t len = npos) const
    // {
    // eostring eostr{"abcdef"};
    // eostring eostr_sub{eostr.substr(2,2)};
    // assert(strcmp(eostr_substr.data(), "cd") == 0);
    // }

    //// size_t copy (char* s, size_t len, size_t pos = 0) const
    // {
    // eostring eostr{"abcdef"};
    // char arr0[1]{};
    // eostr.copy(arr0, 1);
    // assert(strcmp(eostr.data(), "abcdef") == 0);
   
    // char arr1[3]{};
    // eostr.copy(arr0, 3);
    // assert(strcmp(eostr.data(), "abcdef") == 0);
   
    // char arr2[7]{};
    // eostr.copy(arr0, 6);
    // assert(strcmp(eostr.data(), "abcdef") == 0);
    // }


    //// void resize (size_t n)


    //// void swap (eostring& str)

    return 0;
}
