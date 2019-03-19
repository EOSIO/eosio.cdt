#include <iostream>
#include "eostring.hpp"
using namespace std;
int main()
{
    // auto f{ [](eostring& s){cout<<s.size()<<'\n'<<s.capacity()<<'\n'<<s.data()<<"\n\n";} };
    
    //// eostring()
    {
        static const eostring eostr{};
        
        assert(eostr.size()     == 0);
        assert(eostr.capacity() == 0);
        assert(eostr.data()     == nullptr);  
    }

    //// template <size_t N> 
    //// eostring(const char (&str)[N])
    {
        static const eostring eostr0{"a"};
        static const eostring eostr1{"abcdef"};
    
        assert(eostr0.size()              == 1);
        assert(eostr0.capacity()          == 2);
        assert(strcmp(eostr0.data(), "a") == 0);
   
        assert(eostr1.size()                   == 6);
        assert(eostr1.capacity()               == 12);
        assert(strcmp(eostr1.data(), "abcdef") == 0);
    }

    //// eostring(const char* s, const size_t n)
    {
        static const eostring eostr0("a", 0);
        static const eostring eostr1("a", 1);
        static const eostring eostr2("abcdef", 3);
        static const eostring eostr3("abcdefghij", 30);
   
        assert(eostr0.size()             == 0);
        assert(eostr0.capacity()         == 0);
        assert(strcmp(eostr0.data(), "") == 0);

        assert(eostr1.size()              == 1);
        assert(eostr1.capacity()          == 2);
        assert(strcmp(eostr1.data(), "a") == 0);

        assert(eostr2.size()                == 3);
        assert(eostr2.capacity()            == 6);
        assert(strcmp(eostr2.data(), "abc") == 0);

        ////////////////////////////////////////////////
        // This is weird; same behavior with std::string
        assert(eostr3.size()                       == 30);
        assert(eostr3.capacity()                   == 60);
        assert(strcmp(eostr3.data(), "abcdefghij") == 0);
    }

    //// eostring(const size_t n, const char c)
    {
        static const eostring eostr0(0, 'c');
        static const eostring eostr1(1, 'c');
        static const eostring eostr2(3, 'c');
        
        assert(eostr0.size()             == 0);
        assert(eostr0.capacity()         == 0);
        assert(strcmp(eostr0.data(), "") == 0);

        assert(eostr1.size()              == 1);
        assert(eostr1.capacity()          == 2);
        assert(strcmp(eostr1.data(), "c") == 0);

        assert(eostr2.size()                == 3);
        assert(eostr2.capacity()            == 6);
        assert(strcmp(eostr2.data(), "ccc") == 0);
    }

    //// eostring(const eostring& str, const size_t pos, const size_t n = eostring::npos)
    {
        static const eostring eostr{"abcdef"};
        static const eostring eostr0_sub(eostr, 0, 0);
        static const eostring eostr1_sub(eostr, 1, 0);
        static const eostring eostr2_sub(eostr, 0, 1);
        static const eostring eostr3_sub(eostr, 0, 3);
        static const eostring eostr4_sub(eostr, 0, 8);
        static const eostring eostr5_sub(eostr, 0, 7);
        static const eostring eostr6_sub(eostr, 0, 6);
        static const eostring eostr7_sub(eostr, 3, 3);
        static const eostring eostr8_sub(eostr, 3, 2);

        assert(eostr0_sub.size()             == 0);
        assert(eostr0_sub.capacity()         == 0);
        assert(strcmp(eostr0_sub.data(), "") == 0);

        assert(eostr1_sub.size()             == 0);
        assert(eostr1_sub.capacity()         == 0);
        assert(strcmp(eostr1_sub.data(), "") == 0);

        assert(eostr2_sub.size()              == 1);
        assert(eostr2_sub.capacity()          == 2);
        assert(strcmp(eostr2_sub.data(), "a") == 0);

        assert(eostr3_sub.size()                == 3);
        assert(eostr3_sub.capacity()            == 6);
        assert(strcmp(eostr3_sub.data(), "abc") == 0);

        assert(eostr4_sub.size()                   == 6);
        assert(eostr4_sub.capacity()               == 12);
        assert(strcmp(eostr4_sub.data(), "abcdef") == 0);

        assert(eostr5_sub.size()                   == 6);
        assert(eostr5_sub.capacity()               == 12);
        assert(strcmp(eostr5_sub.data(), "abcdef") == 0);

        assert(eostr6_sub.size()                   == 6);
        assert(eostr6_sub.capacity()               == 12);
        assert(strcmp(eostr6_sub.data(), "abcdef") == 0);

        assert(eostr7_sub.size()                == 3);
        assert(eostr7_sub.capacity()            == 6);
        assert(strcmp(eostr7_sub.data(), "def") == 0);

        assert(eostr8_sub.size()               == 2);
        assert(eostr8_sub.capacity()           == 4);
        assert(strcmp(eostr8_sub.data(), "de") == 0);
    }

    //// eostring(const eostring& s)
    {
        static const eostring eostr{"abcdef"};
        static const eostring eostr_cpy(eostr);
   
        assert(eostr_cpy.size()     == eostr.size()     && eostr_cpy.size()    == 6);
        assert(eostr_cpy.capacity() == eostr.capacity() && eostr_cpy.capacity() == 12);
        assert(strcmp(eostr_cpy.data(), "abcdef") == 0);
        assert(eostr.data() != eostr_cpy.data());
    }

    //// eostring(eostring&& s)
    {
        static const eostring eostr0{"abcdef"};
        static const eostring eostr0_mv{std::move(eostr0)};

        assert(eostr0_mv.size()                   == 6);
        assert(eostr0_mv.capacity()               == 12);
        assert(strcmp(eostr0_mv.data(), "abcdef") == 0);
        
        assert(eostr0.size()                   == 6);
        assert(eostr0.capacity()               == 12);
        assert(strcmp(eostr0.data(), "abcdef") == 0);

        assert(eostr0.data() != eostr0_mv.data());
        
        static eostring eostr1{"abcdef"};
        static const eostring eostr1_mv{std::move(eostr1)};

        assert(eostr1_mv.size()                   == 6);
        assert(eostr1_mv.capacity()               == 12);
        assert(strcmp(eostr1_mv.data(), "abcdef") == 0);
        
        assert(eostr1.size()     == 0);
        assert(eostr1.capacity() == 0);
        assert(eostr1.data()     == nullptr);
        
        assert(eostr1.data() != eostr1_mv.data());
    }

    //// eostring& operator=(const eostring& s);
    {
        static const eostring eostr0{"abcdef"};
        static const eostring eostr0_assig = eostr0;

        assert(eostr0_assig.size()     == eostr0.size()     && eostr0_assig.size()     == 6);
        assert(eostr0_assig.capacity() == eostr0.capacity() && eostr0_assig.capacity() == 12);
        assert(strcmp(eostr0_assig.data(), "abcdef") == 0);
        assert(eostr0.data() != eostr0_assig.data());

        eostring eostr1_assig{"abcdef"};

        eostr1_assig = eostr1_assig;
        assert(eostr1_assig.size()     == eostr0.size()     && eostr1_assig.size()     == 6);
        assert(eostr1_assig.capacity() == eostr0.capacity() && eostr1_assig.capacity() == 12);
        assert(strcmp(eostr1_assig.data(), "abcdef") == 0);
        assert(eostr0.data() != eostr1_assig.data());
    }
   
    //// eostring& operator=(eostring&& s)
    {
        static const eostring eostr0{"abcdef"};
        static const eostring eostr0_mv_assig = std::move(eostr0);

        assert(eostr0_mv_assig.size()                   == 6);
        assert(eostr0_mv_assig.capacity()               == 12);
        assert(strcmp(eostr0_mv_assig.data(), "abcdef") == 0);
        
        assert(eostr0.size()                   == 6);
        assert(eostr0.capacity()               == 12);
        assert(strcmp(eostr0.data(), "abcdef") == 0);

        assert(eostr0.data() != eostr0_mv_assig.data());
        
        static eostring eostr1{"abcdef"};
        static const eostring eostr1_mv_assig = std::move(eostr1);

        assert(eostr1_mv_assig.size()                   == 6);
        assert(eostr1_mv_assig.capacity()               == 12);
        assert(strcmp(eostr1_mv_assig.data(), "abcdef") == 0);
        
        assert(eostr1.size()     == 0);
        assert(eostr1.capacity() == 0);
        assert(eostr1.data()     == nullptr);
        
        assert(eostr1.data() != eostr1_mv_assig.data());
    }
   
    //// eostring& operator=(const char* s)
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

    //// eostring& operator+=(const char c)
    {
        static eostring eostr0{};
        static eostring eostr1{"a"};
        static eostring eostr2{"abcdef"};
        
        eostr0 += 'c';
        assert(eostr0.size()              == 2);
        assert(eostr0.capacity()          == 4);
        assert(strcmp(eostr0.data(), "c") == 0);

        eostr1 += 'c';
        eostr1 += 'c';
        assert(eostr1.size()                == 3);
        assert(eostr1.capacity()            == 6);
        assert(strcmp(eostr1.data(), "acc") == 0);

        eostr2 += 'c';
        assert(eostr2.size()                    == 7);
        assert(eostr2.capacity()                == 12);
        assert(strcmp(eostr2.data(), "abcdefc") == 0);
    }

    //// char& at(const size_t n)
    {
        static const eostring eostr{"abcdef"};
        assert(eostr.at(0) == 'a');
        assert(eostr.at(5) == 'f');

        ////////////////////////////////////////////////////////
        // assert(eostr.at(6) == 0); // Edge-case; should throw
    }
      
    //// const char& at(const size_t n) const
    {
        static const eostring eostr{"abcdef"};
        assert(eostr.at(0) == 'a');
        assert(eostr.at(5) == 'f');

        ///////////////////////////////////////////////////////
        // assert(eostr.at(6) == 0); // Edge-case; should throw
    }

    //// char& operator[](const size_t n)
    {
        static const eostring eostr{"abcdef"};
        assert(eostr[0] == 'a');
        assert(eostr[5] == 'f');
    }
      
    //// const char& operator[](const size_t n) const
    {
        static const eostring eostr{"abcdef"};
        assert(eostr[0] == 'a');
        assert(eostr[5] == 'f');
    }
   
    //// char& front()
    {
        static const eostring eostr{"abcdef"};
        assert(eostr.front() == 'a');
    }
      
    //// const char& front() const
    {
        static const eostring eostr{"abcdef"};
        assert(eostr.front() == 'a');
    }
      
    //// char& back()
    {
        static const eostring eostr{"abcdef"};
        assert(eostr.back() == 'f');
    }
      
    //// const char& back() const
    {
        static const eostring eostr{"abcdef"};
        assert(eostr.back() == 'f');
    }
      
    //// char* data()
    {
        static eostring eostr{"abcdef"};
        assert(strcmp(eostr.data(), "abcdef") == 0);

        eostr = "abc";
        assert(strcmp(eostr.data(), "abc") == 0);
    }
      
    //// const char* data() const
    {
        static const eostring eostr{"abcdef"};
        assert(strcmp(eostr.data(), "abcdef") == 0);
    }
      
    //// const char* c_str() const
    {
        static const eostring eostr{"abcdef"};
        assert(strcmp(eostr.c_str(), "abcdef") == 0);
    }

    //// iterator begin()
    {
        static eostring eostr{"abcdef"};
        eostring::iterator iter{eostr.begin()};
        assert(iter   == &eostr[0]);
        assert(iter+1 == &eostr[0]+1);
        assert(iter-1 == &eostr[0]-1);
    }
   
    //// const_iterator cbegin() const
    {
        static const eostring eostr{"abcdef"};
        eostring::const_iterator iter{eostr.cbegin()};
        assert(iter   == &eostr[0]);
        assert(iter+1 == &eostr[0]+1);
        assert(iter-1 == &eostr[0]-1);
    }
   
    //// iterator end()
    {
        static eostring eostr{"abcdef"};
        eostring::iterator iter{eostr.end()};
        assert(iter   == &eostr[eostr.size()]);
        assert(iter+1 == &eostr[eostr.size()+1]);
        assert(iter-1 == &eostr[eostr.size()-1]);
    }
   
    //// const_iterator cend() const
    {
        static const eostring eostr{"abcdef"};
        eostring::const_iterator iter{eostr.cend()};
        assert(iter   == &eostr[eostr.size()]);
        assert(iter+1 == &eostr[eostr.size()+1]);
        assert(iter-1 == &eostr[eostr.size()-1]);
    }

    //// reverse_iterator rbegin()
    {
        // eostring eostr{"abcdef"};
        // eostring::reverse_iterator iter{eostr.rbegin()};
        // assert(iter   == &eostr[eostr.size()-1]);
        // assert(iter+1 == &eostr[eostr.size()-2]);
        // assert(iter-1 == &eostr[eostr.size()]);
    }
   
    //// const_reverse_iterator crbegin() const
    {
        // const eostring eostr{"abcdef"};
        // eostring::const_reverse_iterator iter{eostr.crbegin()};
        // assert(iter   == &eostr[eostr.size()-1]);
        // assert(iter+1 == &eostr[eostr.size()-2]);
        // assert(iter-1 == &eostr[eostr.size()]);
    }
   
    //// reverse_iterator rend()
    {
        // eostring eostr{"abcdef"};
        // eostring::reverse_iterator iter{eostr.rend()};
        // assert(iter   == &eostr[0]-1);
        // assert(iter+1 == &eostr[0]-2);
        // assert(iter-1 == &eostr[0]);
    }
   
    //// const_reverse_iterator crend() const
    {
        // const eostring eostr{"abcdef"};
        // eostring::const_reverse_iterator iter{eostr.crend()};
        // assert(iter   == &eostr[0]-1);
        // assert(iter+1 == &eostr[0]-2);
        // assert(iter-1 == &eostr[0]);
    }

    //// bool eostring::empty() const
    {
        static eostring eostr{};
        assert(eostr.empty() == true);
        eostr += 'c';
        assert(eostr.empty() == false);
    }

    //// size_t eostring::size() const
    {
        static eostring eostr{"abcdef"};
        assert(eostr.size() ==  6);
        eostr += 'g';
        assert(eostr.size() ==  7);
    }

    //// size_t eostring::length() const
    {
        static eostring eostr{"abcdef"};
        assert(eostr.length() ==  6);
        eostr += 'g';
        assert(eostr.length() ==  7);
    }

    //// size_t eostring::capacity() const
    {
        static eostring eostr{"abc"};
        assert(eostr.capacity() ==  6);
        eostr += 'd', eostr += 'e', eostr += 'f';
        assert(eostr.capacity() ==  6);
        eostr += 'g';
        assert(eostr.capacity() ==  14);
    }

    //// size_t eostring::max_size() const
    {
        static const eostring eostr{"abcdef"};
        assert(eostr.max_size() ==  eostring::npos);
    }
   
    //// friend bool operator==(const eostring& lhs, const eostring& rhs)
    {
        static const eostring eostr0{"abc"};
        static const eostring eostr1{"def"};
        assert((eostr0 == eostr0) == true);
        assert((eostr1 == eostr1) == true);
        assert((eostr0 == eostr1) == false);
    }
   
    //// friend bool operator!=(const eostring& lhs, const eostring& rhs)
    {
        static const eostring eostr0{"abc"};
        static const eostring eostr1{"def"};
        assert((eostr0 != eostr0) == false);
        assert((eostr1 != eostr1) == false);
        assert((eostr0 != eostr1) == true);
    }
   
    //// friend bool operator< (const eostring& lhs, const eostring& rhs) // Make sure to double check this logic
    {
        static const eostring eostr0{"abc"};
        static const eostring eostr1{"def"};
        assert((eostr0 < eostr0) == false);
        assert((eostr1 < eostr1) == false);
        assert((eostr0 < eostr1) == true);
    }
   
    //// friend bool operator> (const eostring& lhs, const eostring& rhs)
    {
        static const eostring eostr0{"abc"};
        static const eostring eostr1{"def"};
        assert((eostr0 > eostr0) == false);
        assert((eostr1 > eostr1) == false);
        assert((eostr0 > eostr1) == false);
    }
   
    //// friend bool operator<=(const eostring& lhs, const eostring& rhs)
    {
        static const eostring eostr0{"abc"};
        static const eostring eostr1{"def"};
        assert((eostr0 <= eostr0) == true);
        assert((eostr1 <= eostr1) == true);
        assert((eostr0 <= eostr1) == true);
    }
   
    //// friend bool operator>=(const eostring& lhs, const eostring& rhs)
    {
        static const eostring eostr0{"abc"};
        static const eostring eostr1{"def"};
        assert((eostr0 >= eostr0) == true);
        assert((eostr1 >= eostr1) == true);
        assert((eostr0 >= eostr1) == false);
    }

    //// void eostring::clear()
    {
        static eostring eostr{"abcdef"};
        assert(eostr.empty() == false);
        eostr.clear();
        assert(eostr.empty() == true);
        assert(eostr.size()  == 0);
    }

    //// void reserve(const size_t n)
    {
        static eostring eostr{"abcdef"};
        assert(eostr.capacity() == 12);
        eostr.reserve(10);
        assert(eostr.capacity() == 12);
        eostr.reserve(24);
        assert(eostr.capacity() == 24);
    }

    //// eostring& insert(const size_t pos, const eostring& s)
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
