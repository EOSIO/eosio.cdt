#pragma once

#include "system.hpp"
#include "tree.hpp"

namespace eosio {
   /*
   class simple_string {
      public:
         template <size_t N>
         constexpr simple_string(const char (&str)[N]) {
         }

      private:

   };
   */

   class rope {
      public:
         struct rope_kv {
            size_t      key;
            const char* value;
            bool operator< (rope_kv const& kv)const {
               return key < kv.key;
            }
         };

         template <size_t N>
         constexpr rope(const char (&str)[N]) {
            _store.insert(rope_kv{N-1, str});
            size_in_bytes = N;
         }

         constexpr rope(const char* str) {
            const size_t size = strlen(str);
            _store.insert(rope_kv{size, str});
            size_in_bytes = size;
         }
        
         constexpr rope(const rope& r) : 
            _store(std::move(r._store.clone())), size_in_bytes(r.size_in_bytes)
         {}

         constexpr rope(rope&& r) : 
            _store(std::move(r._store)), size_in_bytes(r.size_in_bytes)
         {}

         constexpr rope& operator= (const rope& r) {
            _store = std::move(r._store.clone());
            size_in_bytes = r.size_in_bytes;
            return *this;
         }

         constexpr rope& operator= (rope&& r) {
            _store = std::move(r._store);
            size_in_bytes = r.size_in_bytes;
            return *this;
         }

         constexpr rope& operator += (const char* str) {
            const size_t size = strlen(str);
            _store.insert(rope_kv{size_in_bytes+size, str});
            size_in_bytes += size;
            return *this;
         }
/*
         constexpr rope& operator += (const char* str) {
            const size_t size = strlen(str);
            _store.insert(rope_kv{size_in_bytes+size, str});
            size_in_bytes += size;
            return *this;
         }
*/
         friend inline rope operator + (rope lhs, const char* rhs) {
            lhs += rhs;
            return lhs;
         }
         /*
         friend constexpr inline rope operator + (rope lhs, const rope& rhs) {
            lhs += rhs;
            return lhs;
         }
         */
         constexpr inline rope& cat(const char* str) {
            return *this += str;
         }

         constexpr inline size_t size()const {
            return size_in_bytes+1;
         }

         void c_str(char* buffer)const {
            std::vector<rope_kv> elems = _store.inorder();
            size_t last_size = 0;
            for (auto elem : elems) {
               size_t size = strlen(elem.value);
               for (int i=0; i < size; i++)
                  buffer[last_size+i] = elem.value[i];
               last_size += size;
            } 
            buffer[size_in_bytes] = '\0';
         }

      private:
         binary_search_tree<rope_kv> _store;
         size_t size_in_bytes = 0;
   };

   template <size_t N>
   class ctstring {
      public:

         static constexpr size_t size = N-1;
         
         constexpr char& operator[]( size_t index ) {
            return _data[index];
         }

         constexpr char operator[]( size_t index )const {
            return _data[index];
         }
         
         template <size_t M>
         constexpr ctstring<N+M> cat(const ctstring<M>& str) {
            ctstring<N+M> ret;
            for (int i=0; i < N-1; i++)
               ret[i] = _data[i];
            for (int i=0; i < M-1; i++)
               ret[N+i] = str[i];
            return ret;
         }

         const char* c_str()const {
            return _data.data();
         }
      private:
         std::array<char, N-1> _data;
   };
} // ns eosio
