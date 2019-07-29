#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#ifndef __STDC_FORMAT_MACROS
# define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>
   
template <uint8_t OS>
int _getExecutablePath(char* out, int capacity, int* dirname_length, typename std::enable_if<OS == sys::_linux, int>::type = 0) {
  char buffer[PATH_MAX];
  char* resolved = NULL;
  int length = -1;

  for (;;)
  {
    resolved = realpath("/proc/self/exe", buffer);
    if (!resolved)
      break;

    length = (int)strlen(resolved);
    if (length <= capacity)
    {
      memcpy(out, resolved, length);

      if (dirname_length)
      {
        int i;

        for (i = length - 1; i >= 0; --i)
        {
          if (out[i] == '/')
          {
            *dirname_length = i;
            break;
          }
        }
      }
    }

    break;
  }

  return length;
}
   
template <uint8_t OS>
int _getModulePath(char* out, int capacity, int* dirname_length, typename std::enable_if<OS == sys::_linux, int>::type = 0) {
  int length = -1;
  FILE* maps = NULL;
  int i;

  for (i = 0; i < 5; ++i)
  {
    maps = fopen("/proc/self/maps", "r");
    if (!maps)
      break;

    for (;;)
    {
      char buffer[PATH_MAX < 1024 ? 1024 : PATH_MAX];
      uint64_t low, high;
      char perms[5];
      uint64_t offset;
      uint32_t major, minor;
      char path[PATH_MAX];
      uint32_t inode;

      if (!fgets(buffer, sizeof(buffer), maps))
        break;

      if (sscanf(buffer, "%" PRIx64 "-%" PRIx64 " %s %" PRIx64 " %x:%x %u %s\n", &low, &high, perms, &offset, &major, &minor, &inode, path) == 8)
      {
        uint64_t addr = (uint64_t)(uintptr_t)__builtin_extract_return_addr(__builtin_return_address(0));
        if (low <= addr && addr <= high)
        {
          char* resolved;

          resolved = realpath(path, buffer);
          if (!resolved)
            break;

          length = (int)strlen(resolved);

          if (length <= capacity)
          {
            memcpy(out, resolved, length);

            if (dirname_length)
            {
              int i;

              for (i = length - 1; i >= 0; --i)
              {
                if (out[i] == '/')
                {
                  *dirname_length = i;
                  break;
                }
              }
            }
          }

          break;
        }
      }
    }

    fclose(maps);

    if (length != -1)
      break;
  }

  return length;
}
