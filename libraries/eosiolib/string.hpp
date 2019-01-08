#pragma once

#include <variant>
#include <set>
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

         constexpr size_t strlen(const char* str)const {
            int i=0;
            while (str[i++]);
            return i;
         }
         /* 
         static void _print(const str_t& s, const std::set<concat_t*>& inserts) {
            prints_l(s.c_str, cc->index - index);
            prints_l(ss.c_str, ss.size-1);
            prints(s.c_str+cc->index - index);
         }

         static void print(const std::unique_ptr<rope_node>& r, int i) {
            static std::set<concat_t*> inserts;
            static int index = i;
            if (r) {
               std::visit(overloaded {
                  [&](concat_t& c) { 
                     if (c.index > 0) {
                        inserts.emplace(&c);
                     } else {
                        if (c.left)
                           print(c.left);
                     }
                     if (c.right)
                        print(c.right);
                  },
                  [&](str_t& s) { 
                     size_t size_so_far = 0;
                     while (inserts.size()) {
                        if ((*inserts.begin())->index < (index + s.size)) {
                           concat_t* cc = (*inserts.begin());
                           auto&     ss = std::get<str_t>(*(cc->left));
                           prints_l(s.c_str, cc->index - index);
                           prints_l(ss.c_str, ss.size-1);
                           prints(s.c_str+cc->index - index);
                           inserts.erase(inserts.begin());
                           size_so_far += s.size + ss.size
                        } else {
                           prints_l(s.c_str, s.size); 
                           index += s.size;
                           break;
                        }
                        index += s.size;
                     }
                  }
               }, *r);
            }
         }

         static void print(const std::unique_ptr<rope_node>& r) {
            print(r, 0);
         }
         */ 
         static void insert(char* buffer, int insert_index, const str_t& s) {
            memcpy(buffer+insert_index+s.size, buffer+insert_index, s.size);
            memcpy(buffer+insert_index, s.c_str, s.size);
         }
         /*
         static void copy_and_insert(char* buffer, size_t& offset, int& index, const str_t& s, std::set<const concat_t*>& inserts) {
            if (inserts.size() && (*inserts.begin())->index < (index + s.size)) {
               auto&  cc  = *inserts.begin();
               auto&  ccs = std::get<str_t>(*(cc->left));
               size_t pre = cc->index - index;
               memcpy(buffer+offset, s.c_str, s.size);
               memcpy(buffer+offset+pre, ccs.c_str, ccs.size-1);
               memcpy(buffer+offset+pre+ccs.size-1, s.c_str+pre, s.size-pre);
               inserts.erase(inserts.begin());
               str_t tmp = {buffer+offset, ccs.size+s.size};
               if (inserts.size())
                  if ((*inserts.begin())->index < index+ccs.size-1+s.size) {
                     copy_and_insert(buffer, offset, index, tmp, inserts);
                  }
               index  += ccs.size-1;
               offset += ccs.size-1;
            } else {
               memcpy(buffer+offset, s.c_str, s.size);
            }
            index += s.size;
            offset += s.size;
         }
         */

         void c_str(char* buffer, const rope_node* r, size_t& offset, std::set<const concat_t*>& inserts)const {
            if (r) {
               std::visit(overloaded {
                  [&](const concat_t& c) { 
                     if (c.index > 0) {
                        inserts.emplace(&c);
                     } else {
                        if (c.left)
                           c_str(buffer, c.left, offset, inserts);
                     }
                     if (c.right) {
                        c_str(buffer, c.right, offset, inserts);
                     }
                  },
                  [&](const str_t& s) { 
                     memcpy(buffer+offset, s.c_str, s.size);
                     offset += s.size;
                  }
               }, *r);
            }
         }

         inline const concat_t& get_cc(const rope_node* rn)const  {
            return std::get<concat_t>(*rn);
         }

         inline const str_t& get_str(const rope_node* rn)const {
            return std::get<str_t>(*rn);
         }

         constexpr void append(const rope_node* n) {
            rope_node* _last = nullptr;
            std::visit(overloaded {
               [&](concat_t& c) { 
                  auto& cc = get_cc(n);
                  auto& ns = get_str(cc.left);
                  c.right = new rope_node(
                       concat_t{ new rope_node(
                            str_t{ns.c_str, ns.size}), nullptr, 0});
                  _last = c.right;
               },
               [](str_t& s) { check(false, "critical failure should not reach here"); }
            }, *last);
            last = _last;
         }
       
         void insert(rope_node* n, size_t index) {
            root = new rope_node{concat_t{n, root, index}};
         }

         rope clone(const rope_node* node)const {
            auto& rs = get_str(get_cc(node).left);
            rope ret(rs.c_str);
            rope_node* n = get_cc(root).right;
            while (n) {
               ret.append(n);
               n = get_cc(n).right;
            }
            ret.last = n;
            ret.size = size;
            return ret;
         }
         
      public:
         rope(const char* s) {
            root = new rope_node(
                     concat_t{new rope_node(str_t{s,strlen(s)-1}), nullptr, 0}
                  );
            last = root;
            size += strlen(s)-1;
         }

         inline rope clone()const {
            return clone(root);
         }
        
         template <size_t N> 
         inline void insert(size_t index, const char (&s)[N]) {
            insert(index, s, N);
         }

         void insert(size_t index, const char* s, size_t len) {
            auto new_node = new rope_node(str_t{s, len});
            root = new rope_node(
                     concat_t{new_node, root, index}
                  );
            size += len;
         }

         template <size_t N> 
         inline constexpr void append(const char (&s)[N]) {
            append(s, N);
         }

         constexpr void append(const char* s, size_t len) {
            str_t _s{s, len};
            append(new rope_node(concat_t{new rope_node(_s), nullptr, 0}));
            size += len;
         }

         constexpr void append(rope&& r) {
            append(std::move(r.root));
            size += r.size;
         }

         constexpr rope& operator+= (const char* s) {
            append(s, strlen(s)-1);
            return *this;
         }

         constexpr rope& operator+= (rope&& r) {
            append(std::move(r));
            return *this;
         }
         
         constexpr size_t length() {
            return size;
         }

         void print() {
            //if (root)
               //print(root);
         }
         
         char* c_str()const {
            char* ret = new char[size+1];
            std::set<const concat_t*> inserts;
            size_t off=0;
            if (root) {
               c_str(ret, root, off, inserts);
               // handle the inserts
               for (auto ins : inserts) {
                  auto& ins_str = std::get<str_t>(*(ins->left));
                  insert(ret, ins->index, ins_str);
               }
            }
            ret[size] = '\0';
            return std::move(ret);
         }
   };

} // ns eosio

inline constexpr bool operator< (const eosio::impl::concat_t& a, const eosio::impl::concat_t& b) {
   return a.index < b.index;
}

