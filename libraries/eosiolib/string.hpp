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
         std::unique_ptr<rope_node> left  = nullptr;
         std::unique_ptr<rope_node> right = nullptr;
         size_t                     index = 0;
      };
   }

   using namespace impl;
   class rope {
      private:
         std::unique_ptr<rope_node> root = nullptr;
         rope_node* last                 = nullptr;
         size_t size                     = 0;

         constexpr size_t strlen(const char* str)const {
            int i=0;
            while (str[i++]);
            return i;
         }
         
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
         
         static void copy_and_insert(char* buffer, int index, const str_t& s, size_t sz, const concat_t* cc) {
            auto&  ccs = std::get<str_t>(*(cc->left));
            size_t pre = cc->index - index;
            memcpy(buffer+offset, s.c_str, pre);
            memcpy(buffer+offset+pre, ss.c_str, ss.size-1);
            memcpy(buffer+offset+pre+ss.size-1, s.c_str+pre, s.size-pre);
         }

         static void c_str(char* buffer, const std::unique_ptr<rope_node>& r, size_t& offset, int i) {
            static std::set<concat_t*> inserts;
            static int index = i;
            if (r) {
               std::visit(overloaded {
                  [&](concat_t& c) { 
                     if (c.index > 0) {
                        inserts.emplace(&c);
                     } else {
                        if (c.left)
                           c_str(buffer, c.left, offset, i);
                     }
                     if (c.right)
                        c_str(buffer, c.right, offset, i);
                  },
                  [&](str_t& s) { 
                     if (inserts.size() && (*inserts.begin())->index < (index + s.size)) {
                        concat_t* cc = (*inserts.begin());
                        _c_str(buffer, index, s, cc);
                        inserts.erase(inserts.begin());
                        index += ss.size;
                        offset += ss.size;
                     } else {
                        memcpy(buffer+offset, s.c_str, s.size-1); 
                     }
                     index += s.size;
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
         
         inline const concat_t& get_cc(const std::unique_ptr<rope_node>& rn)const {
            return get_cc(rn.get());
         }

         inline const str_t& get_str(const std::unique_ptr<rope_node>& rn)const {
            return get_str(rn.get());
         }

         constexpr void append(const rope_node* n) {
            rope_node* _last = nullptr;
            std::visit(overloaded {
               [&](concat_t& c) { 
                  auto& cc = get_cc(n);
                  auto& ns = get_str(cc.left);
                  c.right = std::make_unique<rope_node>(
                       concat_t{std::make_unique<rope_node>(
                            str_t{ns.c_str, ns.size}), nullptr, 0});
                  _last = c.right.get();
               },
               [](str_t& s) { check(false, "critical failure should not reach here"); }
            }, *last);
            last = _last;
         }

         inline constexpr void append(const std::unique_ptr<rope_node>& n) {
            append(n.get());
         }

         constexpr void append(std::unique_ptr<rope_node>&& n) {
            if (!last) {
               root = std::move(n);
               last = n.get();
               append(std::move(std::get<concat_t>(*(std::get<concat_t>(*n).right)).right));
               return;
            }
            rope_node* _last = n.get();
            std::visit(overloaded {
               [&, this](concat_t& c) { 
                  c.right = std::move(n); 

                  if (c.right != nullptr) {
                     last = _last;
                     if (get_cc(c.right).right != nullptr)
                        append(std::move(std::get<concat_t>(*(c.right)).right));
                  }
               },
               [](str_t& s) { check(false, "critical failure should not reach here"); }
            }, *last);
         }
         
         constexpr void insert(std::unique_ptr<rope_node>&& n, size_t index) {
            root = std::make_unique<rope_node>(
                     concat_t{std::move(n), std::move(root), index}
                  );
         }

         rope copy(const std::unique_ptr<rope_node>& node)const {
            auto& rs = get_str(get_cc(node).left);
            rope ret(rs.c_str);
            rope_node* n = get_cc(root).right.get();
            while (n) {
               ret.append(n);
               n = get_cc(n).right.get();
            }
            ret.last = n;
            ret.size = size;
            return ret;
         }
         
      public:
         constexpr rope(const char* s) {
            root = std::make_unique<rope_node>(
                     concat_t{std::make_unique<rope_node>(str_t{s,strlen(s)-1}), nullptr, 0}
                  );
            last = root.get();
            size += strlen(s)-1;
         }
        
         constexpr rope(const rope& r) {
            auto& cc = get_cc(r.root);
            auto& s = get_str(cc.left);
            root = std::make_unique<rope_node>(
                     concat_t{std::make_unique<rope_node>(str_t{s.c_str, s.size}), nullptr, 0}
                  );
            last = root.get();
            append(r.copy());
            size = r.size;
         }
         
         constexpr rope(rope&& r) {
            root = std::move(r.root);
            last = root.get();
            size = r.size;
         }

         constexpr rope& operator= (const rope& r) {
            append(std::move(r.copy()));
            return *this;
         }

         constexpr rope& operator= (rope&& r) {
            append(std::move(r));
            return *this;
         }

         inline rope copy()const {
            return copy(root);
         }
        
         template <size_t N> 
         inline void insert(size_t index, const char (&s)[N]) {
            insert(index, s, N);
         }

         void insert(size_t index, const char* s, size_t len) {
            auto new_node = std::make_unique<rope_node>(str_t{s, len});
            root = std::make_unique<rope_node>(
                     concat_t{std::move(new_node), std::move(root), index}
                  );
            size += len;
         }

         template <size_t N> 
         inline constexpr void append(const char (&s)[N]) {
            append(s, N);
         }

         constexpr void append(const char* s, size_t len) {
            str_t _s{s, len};
            append(std::make_unique<rope_node>(concat_t{std::make_unique<rope_node>(_s), nullptr, 0}));
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
            if (root)
               print(root);
         }
         
         std::unique_ptr<char[]> c_str()const {
            std::unique_ptr<char[]> ret{new char[size+1]};
            size_t off=0;
            if (root)
               c_str(ret.get(), root, off, 0);
            ret.get()[size] = '\0';
            return std::move(ret);
         }
   };

} // ns eosio

inline constexpr bool operator< (const eosio::impl::concat_t& a, const eosio::impl::concat_t& b) {
   return a.index < b.index;
}

