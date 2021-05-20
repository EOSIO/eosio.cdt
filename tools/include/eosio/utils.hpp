#pragma once

#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/Support/Program.h"

#include <stdlib.h>
#if defined(__APPLE__)
# include <crt_externs.h>
#elif !defined(_MSC_VER)
// Forward declare environ in case it's not provided by stdlib.h.
extern char **environ;
#endif

#include "whereami/whereami.hpp"
#include <vector>
#include <sstream>

namespace eosio { namespace cdt {

uint64_t char_to_symbol( char c ) {
   if( c >= 'a' && c <= 'z' )
      return (c - 'a') + 6;
   if( c >= '1' && c <= '5' )
      return (c - '1') + 1;
   return 0;
}

uint64_t string_to_name( const char* str )
{
   uint64_t name = 0;
   int i = 0;
   for ( ; str[i] && i < 12; ++i) {
       // NOTE: char_to_symbol() returns char type, and without this explicit
       // expansion to uint64 type, the compilation fails at the point of usage
       // of string_to_name(), where the usage requires constant (compile time) expression.
        name |= (char_to_symbol(str[i]) & 0x1f) << (64 - 5 * (i + 1));
    }

   // The for-loop encoded up to 60 high bits into uint64 'name' variable,
   // if (strlen(str) > 12) then encode str[12] into the low (remaining)
   // 4 bits of 'name'
   if (i == 12)
       name |= char_to_symbol(str[12]) & 0x0F;
   return name;
}

template <typename Lambda>
void validate_name( const std::string& str, Lambda&& error_handler ) {
   const auto len = str.length();
   if ( len > 13 ) {
      return error_handler(std::string("Name {")+str+"} is more than 13 characters long");
   }
   uint64_t value = string_to_name( str.c_str() );

   static const char* charmap = ".12345abcdefghijklmnopqrstuvwxyz";
   std::string str2(13,'.');

   uint64_t tmp = value;
   for( uint32_t i = 0; i <= 12; ++i ) {
      char c = charmap[tmp & (i == 0 ? 0x0f : 0x1f)];
      str2[12-i] = c;
      tmp >>= (i == 0 ? 4 : 5);
   }

   auto trim = [](std::string& s) {
      int i;
      for (i = s.length()-1; i >= 0; i--)
         if (s[i] != '.')
            break;
      s = s.substr(0,i+1);
   };
   trim(str2);

   if ( str2 != str ) {
      return error_handler("name not properly normalized");
   }
}

std::string name_to_string( uint64_t nm ) {
   static const char* charmap = ".12345abcdefghijklmnopqrstuvwxyz";
   std::string str(13,'.');

   uint64_t tmp = nm;
   for( uint32_t i = 0; i <= 12; ++i ) {
      char c = charmap[tmp & (i == 0 ? 0x0f : 0x1f)];
      str[12-i] = c;
      tmp >>= (i == 0 ? 4 : 5);
   }

   auto trim_right_dots = [](std::string& str) {
      const auto last = str.find_last_not_of('.');
      if (last != std::string::npos)
         str = str.substr(0, last+1);
   };
   trim_right_dots( str );
   return str;
}

struct environment {
   static llvm::ArrayRef<llvm::StringRef> get() {
      static std::vector<llvm::StringRef> env_table;
      std::vector<std::string>     env_storage;
       auto envp = []{
#if defined(_WIN32)
         _wgetenv(L"TMP"); // Populate _wenviron, initially is null
         return _wenviron;
#elif defined(__APPLE__)
         return *_NSGetEnviron();
#else
         return environ;
#endif
       }();

       auto prepareEnvVar = [&](decltype(*envp) var) -> llvm::StringRef {
#if defined(_WIN32)
         // On Windows convert UTF16 encoded variable to UTF8
         auto len = wcslen(var);
         llvm::ArrayRef<char> ref{reinterpret_cast<char const *>(var),
                            len * sizeof(*var)};
         env_storage.emplace_back();
         auto convStatus = convertUTF16ToUTF8String(ref, env_storage.back());
         return env_storage.back();
#else
         return llvm::StringRef(var);
#endif
       };

       while (*envp != nullptr) {
         env_table.emplace_back(prepareEnvVar(*envp));
         ++envp;
       }
     return env_table;
   }
   static bool exec_subprogram(const std::string prog, std::vector<std::string> options, bool root=false) {
      std::vector<llvm::StringRef> args;
      args.push_back(prog);
      args.insert(args.end(), options.begin(), options.end());
      std::string find_path = eosio::cdt::whereami::where();
      if (root)
         find_path = "/usr/bin";
      if ( const auto& path = llvm::sys::findProgramByName(prog.c_str(), {find_path}) ) {
         return llvm::sys::ExecuteAndWait(*path, args, {}, {}, 0, 0, nullptr, nullptr) == 0;
      }
      else
         return false;
      return true;
   }

};
}} // ns eosio::cdt
