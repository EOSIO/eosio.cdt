#define _DARWIN_BETTER_REALPATH
#include <mach-o/dyld.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

template <uint8_t OS>
int _getExecutablePath(char* out, int capacity, int* dirname_length, typename std::enable_if<OS == sys::_osx, int>::type = 0) {
     char buffer1[PATH_MAX];
     char buffer2[PATH_MAX];
     char* path = buffer1;
     char* resolved = NULL;
     int length = -1;

     for (;;)
     {
       uint32_t size = (uint32_t)sizeof(buffer1);
       if (_NSGetExecutablePath(path, &size) == -1)
       {
         path = (char*)malloc(size);
         if (!_NSGetExecutablePath(path, &size))
           break;
       }

       resolved = realpath(path, buffer2);
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

     if (path != buffer1)
       free(path);

     return length;
   }

template <uint8_t OS>
int _getModulePath(char* out, int capacity, int* dirname_length, typename std::enable_if<OS == sys::_osx, int>::type = 0) {
  char buffer[PATH_MAX];
  char* resolved = NULL;
  int length = -1;

  for(;;)
  {
    Dl_info info;

    if (dladdr(__builtin_extract_return_addr(__builtin_return_address(0)), &info))
    {
      resolved = realpath(info.dli_fname, buffer);
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
    }

    break;
  }

  return length;
}
