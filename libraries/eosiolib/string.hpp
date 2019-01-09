#pragma once

#include <variant>
#include <set>
#include <stack>
#include "system.hpp"
#include "tree.hpp"

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
         size_t     index = 0;
      };
   }

   using namespace impl;
   class rope {
      private:
         rope_node* root = nullptr;
         rope_node* last = nullptr;
         size_t size     = 0;
         
         static constexpr void print(const rope_node* rn) {
            if (auto val = std::get_if<concat_t>(rn))
               eosio::print_f("concat % % %\n", (size_t)val->left, (size_t)val->right, val->index);
            else if (auto val = std::get_if<str_t>(rn))
               eosio::print_f("str %\n", val->c_str);
         }
         static constexpr size_t strlen(const char* str) {
            int i=0;
            while (str[i++]);
            return i;
         }

         constexpr void insert(char* buffer, int insert_index, const str_t& s)const {
            memmove(buffer+insert_index+s.size, buffer+insert_index, strlen(buffer+insert_index));
            memcpy(buffer+insert_index, s.c_str, s.size);
         }

         void c_str(char* buffer, const rope_node* r, size_t& offset)const {
            // the natural recursive version of this function causes call depth maximum assert to be thrown from nodeos
            std::set<const concat_t*> inserts;
            std::stack<const rope_node*> last_cc;
            const rope_node* tmp = r;
            while (true) {
               if (!tmp || tmp == last) {
                  if (!last_cc.empty()) {
                     tmp = last_cc.top();
                     last_cc.pop();
                     continue;
                  }
                  break;
               }
               const auto val = std::get_if<concat_t>(tmp);
               eosio::check(val, "rope c_str critical failure");
               eosio::print("----------\n");
               print(val->left);
               print(val->right);
               eosio::print("----------\n");

               if (std::holds_alternative<concat_t>(*(val->left))) {
                  last_cc.push(val->right);
                  tmp = val->left;
               } else {
                  if (val->index > 0) {
                     inserts.insert(val);
                  } else {
                     if (val->left) {
                        if (auto lval = std::get_if<str_t>(val->left)) {
                           memcpy(buffer+offset, lval->c_str, lval->size);
                           offset += lval->size;
                        }
                     }
                  }
                  tmp = val->right;
               }
            }
            eosio::print_f("OFF %\n", offset);
            for (auto ins : inserts) {
               auto& ins_str = std::get<str_t>(*(ins->left));
               insert(buffer, ins->index, ins_str);
            }
         }

         inline constexpr const concat_t& get_cc(const rope_node* rn)const  {
            return std::get<concat_t>(*rn);
         }

         inline constexpr const str_t& get_str(const rope_node* rn)const {
            return std::get<str_t>(*rn);
         }

         constexpr void insert(rope_node* n, size_t index) {
            root = new rope_node{concat_t{n, root, index}};
         }

         constexpr void append(rope_node* rn, bool use_left=false) {
            auto cc = std::get_if<concat_t>(last);
            eosio::check(cc, "append should only allow concat nodes");
            if (use_left) {
               root = new rope_node(concat_t{root, rn, 0});
            }
            else
               cc->right = rn;
         }

      public:
         constexpr rope(const char* s) {
            root = new rope_node(concat_t{new rope_node(str_t{s,strlen(s)-1}), nullptr, 0});
            last = root;
            size += strlen(s)-1;
         }

         template <size_t N> 
         inline constexpr void insert(size_t index, const char (&s)[N]) {
            insert(index, s, N-1);
         }

         void constexpr insert(size_t index, const char* s, size_t len) {
            auto new_node = new rope_node(str_t{s, len});
            root = new rope_node(concat_t{new_node, root, index});
            size += len;
         }

         template <size_t N> 
         inline constexpr void append(const char (&s)[N]) {
            append(s, N);
         }

         constexpr void append(const char* s, size_t len) {
            str_t _s{s, len};
            rope_node* rn = new rope_node(concat_t{new rope_node(_s), nullptr, 0});
            append(rn);
            last = rn;
            size += len;
         }

         constexpr void append(const rope& r) {
            eosio::print("append(const rope& r)\n");
            append(r.root, true);
            size += r.size;
            last = r.last;
         }
        
         constexpr void append(rope&& r) {
            eosio::print("append(rope&& r)\n");
            append(std::move(r.root));
            size += r.size;
            last = r.last;
         }

         constexpr rope& operator+= (const char* s) {
            append(s, strlen(s)-1);
            return *this;
         }

         constexpr rope& operator+= (rope&& r) {
            eosio::print("operator+=(rope&& r)\n");
            append(std::move(r));
            return *this;
         }

         constexpr rope& operator+= (const rope& r) {
            eosio::print("operator+=(const rope& r)\n");
            append(r);
            return *this;
         }
         
         friend rope operator+ (rope lhs, const char* s) {
            lhs += rope(s);
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

         void print() {
            eosio::print(c_str());
         }
         
         constexpr char* c_str()const {
            char* ret = new char[size+1];
            size_t off=0;
            eosio::print_f("SIZE %\n", size);
            if (root) {
               c_str(ret, root, off);
            }
            ret[size] = '\0';
            return ret;
         }
   };

} // ns eosio

inline constexpr bool operator< (const eosio::impl::concat_t& a, const eosio::impl::concat_t& b) {
   return a.index < b.index;
}

