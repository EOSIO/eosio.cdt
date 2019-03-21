#pragma once

#include <variant>
#include <set>
#include <stack>
#include <string_view>
#include "check.hpp"
#include "print.hpp"

namespace eosio {
   template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
   template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

   namespace impl {
      struct str_t;
      struct concat_t;
      typedef std::variant<concat_t, str_t> rope_node;

      struct str_t {
         const char*              c_str;
         size_t                   size;
         mutable const rope_node* temp = nullptr;
      };
      
      struct concat_t {
         rope_node*               left      = nullptr;
         rope_node*               right     = nullptr;
         mutable const rope_node* temp      = nullptr;
         mutable bool             temp_left = false;
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
         
         static void c_str(char* buffer, const rope_node* r, size_t& off) {
            bool going_down = true;
            if (r) {
               std::visit([](const auto& n) {
                  n.temp = nullptr;
               }, *r);
            }
            while (r) {
               if (going_down) {
                  std::visit(overloaded {
                     [&](const concat_t& c) {
                        if (c.left) {
                           c.temp_left = true;
                           std::visit([&](const auto& n){
                              n.temp = r;
                           }, *c.left);
                           going_down = true;
                           r = c.left;
                        } else if (c.right) {
                           c.temp_left = false;
                           std::visit([&](const auto& n){
                              n.temp = r;
                           }, *c.right);
                           going_down = true;
                           r = c.right;
                        } else {
                           going_down = false;
                           r = c.temp;
                        }
                     },
                     [&](const str_t& s) { 
                        off -= s.size;
                        memcpy(buffer+off, s.c_str, s.size);
                        going_down = false;
                        r = s.temp;
                     }
                  }, *r);
               } else {
                  const auto& c = std::get<concat_t>(*r);
                  if (c.temp_left && c.right) {
                     c.temp_left = false;
                     std::visit([&](const auto& n){
                        n.temp = r;
                     }, *c.right);
                     going_down = true;
                     r = c.right;
                  } else {
                     going_down = false;
                     r = c.temp;
                  }
               }
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

         rope(std::string_view s = "") {
            root = new rope_node(concat_t{new rope_node(str_t{s.begin(),s.size()}), nullptr});
            size += s.size();
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

         std::string_view sv()const {
            return {c_str(), size};
         }
   };
} // ns eosio
