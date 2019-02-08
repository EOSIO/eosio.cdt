#pragma once

#include <variant>
#include <set>
#include <stack>
#include "check.hpp"

namespace eosio {
   template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
   template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

   namespace impl {
      struct str_t {
         const char* c_str;
         size_t      size;
      };
      
      struct concat_t;
      typedef std::variant<concat_t, str_t> rope_node;
      struct concat_t {
         rope_node* left  = nullptr;
         rope_node* right = nullptr;
      };
   }

   using namespace impl;
   class rope {
      private:
         rope_node* root = nullptr;
         rope_node* last = nullptr;
         size_t size     = 0;
         
         static constexpr size_t strlen(const char* str) {
            int i=0;
            while (str[i++]);
            return i;
         }
         
         void c_str(char* buffer, const rope_node* r, size_t& off)const {
            if (r) {
               std::visit(overloaded {
                  [&](const concat_t& c) {
                     if (c.left)
                        c_str(buffer, c.left, off);
                     if (c.right)
                        c_str(buffer, c.right, off);
                  },
                  [&](const str_t& s) { 
                     off -= s.size;
                     memcpy(buffer+off, s.c_str, s.size);
                  }
               }, *r);
            }
         }

         void append(rope_node* rn, bool use_left=false) {
            auto cc = std::get_if<concat_t>(rn);
            eosio::check(cc, "append should only allow concat nodes");
            if (use_left)
               root = new rope_node(concat_t{rn, root});
            else {
               cc->right = root;
               root = rn;
            }
         }

         void at(char& ret, size_t index, rope_node* rn, size_t& index_so_far, bool& stop) {
            if (rn) {
               std::visit(overloaded {
                  [&](const concat_t& c) {
                     if (!stop) {
                        if (c.left)
                           at(ret, index, c.left, index_so_far, stop);
                        if (c.right)
                           at(ret, index, c.right, index_so_far, stop);
                     }
                  },
                  [&](const str_t& s) { 
                     index_so_far -= s.size;
                     if (index >= index_so_far) {
                        ret = s.c_str[index-index_so_far];
                        stop = true;
                     }
                  }
               }, *rn);
            }
         }

      public:
         rope(const char* s) {
            root = new rope_node(concat_t{new rope_node(str_t{s,strlen(s)-1}), nullptr});
            last = root;
            size += strlen(s)-1;
         }

         template <size_t N> 
         inline constexpr void append(const char (&s)[N]) {
            append(s, N);
         }

         void append(const char* s, size_t len) {
            str_t _s{s, len};
            rope_node* rn = new rope_node(concat_t{new rope_node(_s), nullptr});
            append(rn);
            size += len;
         }
         
         constexpr char at(size_t index) {
            char ret = '\0';
            size_t sz = size;
            bool stop = false;
            at(ret, index, root, sz, stop);
            return ret;
         }

         constexpr void append(const rope& r) {
            append(r.root, true);
            size += r.size;
         }
        
         constexpr void append(rope&& r) {
            append(r.root, true);
            size += r.size;
         }

         constexpr char operator[](size_t index) {
            return at(index);
         }

         constexpr rope& operator+= (const char* s) {
            append(s, strlen(s)-1);
            return *this;
         }

         constexpr rope& operator+= (const rope& r) {
            append(r);
            return *this;
         }
         constexpr rope& operator+= (rope&& r) {
            append(std::move(r));
            return *this;
         }

         friend rope operator+ (rope lhs, const char* s) {
            lhs += s;
            return lhs;
         }

         friend rope operator+ (rope lhs, const rope& rhs) {
            lhs += rhs;
            return lhs;
         } 

         friend rope operator+ (rope lhs, rope&& rhs) {
            lhs += std::move(rhs);
            return lhs;
         } 
         
         
         constexpr size_t length() {
            return size;
         }
         
         void print()const {
            eosio::print(c_str());
         }
         
         char* c_str()const {
            char* ret = new char[size+1];
            if (root) {
               size_t off = size;
               c_str(ret, root, off);
            }
            ret[size] = '\0';
            return ret;
         }
   };
} // ns eosio
