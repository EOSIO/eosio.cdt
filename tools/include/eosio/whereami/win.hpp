#define WIN32_LEAN_AND_MEAN
#if defined(_MSC_VER)
#pragma warning(push, 3)
#endif
#include <windows.h>
#include <intrin.h>
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
#endif

template <uint8_t OS>
int _getModulePath(HMODULE module, char* out, int capacity, int* dirname_length, typename std::enable_if<OS == sys::_win, int>::type = 0) {
   wchar_t buffer1[MAX_PATH];
   wchar_t buffer2[MAX_PATH];
   wchar_t* path = NULL;
   int length = -1;

   for (;;)
   {
     DWORD size;
     int length_, length__;

     size = GetModuleFileNameW(module, buffer1, sizeof(buffer1) / sizeof(buffer1[0]));

     if (size == 0)
       break;
     else if (size == (DWORD)(sizeof(buffer1) / sizeof(buffer1[0])))
     {
       DWORD size_ = size;
       do
       {
         wchar_t* path_;

         path_ = (wchar_t*)realloc(path, sizeof(wchar_t) * size_ * 2);
         if (!path_)
           break;
         size_ *= 2;
         path = path_;
         size = GetModuleFileNameW(module, path, size_);
       }
       while (size == size_);

       if (size == size_)
         break;
     }
     else
       path = buffer1;

     if (!_wfullpath(buffer2, path, MAX_PATH))
       break;
     length_ = (int)wcslen(buffer2);
     length__ = WideCharToMultiByte(CP_UTF8, 0, buffer2, length_ , out, capacity, NULL, NULL);

     if (length__ == 0)
       length__ = WideCharToMultiByte(CP_UTF8, 0, buffer2, length_, NULL, 0, NULL, NULL);
     if (length__ == 0)
       break;

     if (length__ <= capacity && dirname_length)
     {
       int i;

       for (i = length__ - 1; i >= 0; --i)
       {
         if (out[i] == '\\')
         {
           *dirname_length = i;
           break;
         }
       }
     }

     length = length__;

     break;
   }

   if (path != buffer1)
     free(path);

   return length;
   }

template <uint8_t OS>
int _getExecutablePath(char* out, int capacity, int* dirname_length, typename std::enable_if<OS == sys::_win, int>::type = 0) {
   return _getModulePath(NULL, out, capacity, dirname_length);
}

template <uint8_t OS>
int _getModulePath(char* out, int capacity, int* dirname_length, typename std::enable_if<OS == sys::windows, int>::type = 0) {
   HMODULE module;
   int length = -1;

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4054)
#endif
   if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR)WAI_RETURN_ADDRESS(), &module))
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
   {
     length = getModulePath(module, out, capacity, dirname_length);
   }

   return length;
   }
}
