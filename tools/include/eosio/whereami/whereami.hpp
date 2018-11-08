#pragma once

namespace eosio { namespace cdt {
enum sys {
   _win,
   _linux,
   _osx,
   _bsd
};

#include <stdlib.h>
#if defined(_WIN32)
#include "win.hpp"
#define __EOSIO_OS__ sys::_win

#elif defined(__linux__) || defined(__CYGWIN__)
#include "linux.hpp"
#define __EOSIO_OS__ sys::_linux

#elif defined(__APPLE__)
#include "osx.hpp"
#define __EOSIO_OS__ sys::_osx

#elif defined(__DragonFly__) || defined(__FreeBSD__) || \
      defined(__FreeBSD_kernel__) || defined(__NetBSD__)
#include "bsd.hpp"
#define __EOSIO_OS__ sys::_bsd

#else
#error unsupported platform
#endif

struct whereami {
   static inline int getExecutablePath(char* out, int capacity, int* dirname_length) {
      return _getExecutablePath<__EOSIO_OS__>(out, capacity, dirname_length);
   }

   static inline int getModulePath(char* out, int capacity, int* dirname_length) {
      return _getModulePath<__EOSIO_OS__>(out, capacity, dirname_length);
   }

   static std::string where() {
      static bool cached = false;
      static std::vector<char> ret;
      if (cached)
         return {ret.data()};

      int dirname_len = 0;
      auto len = whereami::getExecutablePath(NULL, 0, &dirname_len);
      if (len <= 0) 
         return "";
      ret.resize(len+1);
      whereami::getExecutablePath(ret.data(), len, &dirname_len);
      ret[dirname_len] = '\0';
      cached = true;
      return {ret.data()};
   }
};

}} // ns eosio::cdt
