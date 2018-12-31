#pragma once

#include <variant>
#include "system.hpp"
#include "tree.hpp"

namespace eosio {
   template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
   template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

   class rope {
      private:
         struct str {
            const char* c_str;
            size_t      size;
         };
         struct concat_t;
         typedef std::variant<concat_t, str> rope_node;
         struct concat_t {
            std::unique_ptr<rope_node> left  = nullptr;
            std::unique_ptr<rope_node> right = nullptr;
         };

         std::unique_ptr<rope_node> root = nullptr;
         rope_node* last = nullptr;
         //binary_search_tree<rope_kv> _store;
         size_t size_in_bytes = 0;
         constexpr size_t strlen(const char* str)const {
            int i=0;
            while (str[i++]);
            return i;
         }
         static void print(const std::unique_ptr<rope_node>& r) {
            if (r) {
               std::visit(overloaded {
                  [](concat_t& c) { 
                     if (c.left)
                        print(c.left);
                     if (c.right)
                        print(c.right);
                  },
                  [](str& s) { prints_l(s.c_str, s.size); }
               }, *r);
            }
         }
/*
         constexpr void copy(rope_node& ret, const std::unique_ptr<rope_node>& r) {
            std::visit(overloaded {
               [](concat_t& c) { c.left); copy(*(ret.right), c.right); },
               [](str& s) { *ret =  }
            }, *r);
         }
      public:
         struct rope_kv {
            size_t      key;
            const char* value;
            bool operator< (rope_kv const& kv)const {
               return key < kv.key;
            }
         };
/*
         template <size_t N>
         constexpr rope(const char (&str)[N]) {
            auto str_node = std::make_unique<rope_node>(str, N);
            auto cc_node  = std::make_unique<rope_node>(
            root = std::move(new_node);
         }
         */
         constexpr void append(std::unique_ptr<rope_node>&& n) {
            rope_node* _last = n.get();
            std::visit(overloaded {
               [&](concat_t& c) { 
                  c.right = std::move(n); 
               },
               [](str& s) { check(false, "critical failure should not reach here"); }
            }, *last);
            last = _last;
         }

      public:

         rope(const char* s) {
            root = std::make_unique<rope_node>(
                     concat_t{std::make_unique<rope_node>(str{s,strlen(s)}), nullptr}
                  );
            last = root.get();
         }
        
         rope(const rope& r) //:
         {}
         
         rope(rope&& r) //: 
            //_store(std::move(r._store)), size_in_bytes(r.size_in_bytes)
         {}
         
         rope copy() {
         }

         constexpr rope& operator= (const rope& r) {
            //_store = std::move(r._store.clone());
            size_in_bytes = r.size_in_bytes;
            return *this;
         }

         constexpr rope& operator= (rope&& r) {
            //_store = std::move(r._store);
            size_in_bytes = r.size_in_bytes;
            return *this;
         }
         
         void insert(const char* s) {
            str _s{s, strlen(s)};
            auto new_node = std::make_unique<rope_node>(_s);
         }

         constexpr void append(const char* s) {
            str _s{s, strlen(s)};
            append(std::make_unique<rope_node>(concat_t{std::make_unique<rope_node>(_s), nullptr}));
         }

         constexpr void append(rope&& r) {
            append(std::move(r.root));
         }

         constexpr rope& operator+= (const char* s) {
            append(s);
            return *this;
         }

         constexpr rope& operator+= (rope&& r) {
            append(std::move(r));
            return *this;
         }

         friend inline rope operator+ (rope& lhs, const char* rhs) {
            return (lhs += rhs);
         }
         /*
         friend inline rope operator+ (rope lhs, const char* rhs) {
            lhs += rhs;
            return std::move(lhs);
         }
         */

         /*
         friend constexpr inline rope operator + (rope lhs, const rope& rhs) {
            lhs += rhs;
            return lhs;
         }
         */

         constexpr inline size_t size()const {
            return size_in_bytes+1;
         }

         void print() {
            if (root)
               print(root);
         }
         void c_str(char* buffer)const {
            /*
            std::vector<rope_kv> elems = _store.inorder();
            size_t last_size = 0;
            for (auto elem : elems) {
               size_t size = strlen(elem.value);
               for (int i=0; i < size; i++)
                  buffer[last_size+i] = elem.value[i];
               last_size += size;
            } 
            buffer[size_in_bytes] = '\0';
            */
         }

   };

} // ns eosio
