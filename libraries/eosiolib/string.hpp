#pragma once

//#include <string_view>
#include <variant>
#include "system.hpp"
#include "tree.hpp"

namespace eosio {
   template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
   template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

   namespace detail {
      struct string_node_base {
         const char* str  = nullptr;
         uint32_t    size = 0;
         //std::unique_ptr<string_node_base[]> base_nodes = nullptr;
      };
      
      struct string_node; 
      struct string_node {
         string_node* next = nullptr;
         const char* str   = nullptr;
         uint32_t size     = 0;
      };

      struct internal_storage {
         internal_storage() {
            root = new string_node();
            tail = root;
         }

         inline constexpr void append(const char* s, uint32_t sz) {
            tail->str  = s;
            tail->size = sz;
            tail->next = new string_node();
            tail = tail->next;
         }

         inline constexpr void append(internal_storage&& is) {
            tail->str  = is.root->str;
            tail->size = is.root->size;
            tail->next = new string_node();
            tail = tail->next;
         }

         inline constexpr string_node& at(size_t index) {
            string_node* n = root; 
            for (int i=0; i < index+1; i++) {
               n = n->next;
               check(n, "at failed, null pointer");
            }
            return *n;
         }
         
         inline constexpr string_node& back() {
            return *tail;
         }

         string_node* root = nullptr; 
         string_node* tail = nullptr;
      };
   }

   class string {
      public:
         
         inline constexpr string(const char* str) {
            storage.append(str, strlen(str));
         }
         
         inline constexpr void append(const char* str) {
            storage.append(str, strlen(str));
         }

         inline constexpr void append(string&& str) {
            storage.append(std::move(str.storage));
         }

         inline constexpr string& operator+= (const char* str) {
            append(str);
            return *this;
         }

         inline constexpr string& operator+= (string&& r) {
            append(std::move(r));
            return *this;
         }

         inline friend inline string operator+ (string& lhs, const char* rhs) {
            return (lhs += rhs);
         }
        
         inline constexpr char at(size_t index) {
            return _at(index, 0);
         }

      private:
         inline constexpr size_t strlen(const char* str)const {
            int i=0;
            while (str[i++]);
            return i;
         }

         constexpr char _at(size_t index, size_t n) {
            return 'c';
            /*
            if (storage.at(n).size < index)
               return storage.at(n).base_nodes[index];
            else
               return _at(index, n+1);
               */
         }

         detail::internal_storage storage; // start with 16 slots for strings
   };

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
         rope_node* last                 = nullptr;
         size_t size                     = 0;

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

         static void c_str(char* buffer, const std::unique_ptr<rope_node>& r, size_t& offset) {
            if (r) {
               std::visit(overloaded {
                  [&](concat_t& c) { 
                     if (c.left)
                        c_str(buffer, c.left, offset);
                     if (c.right)
                        c_str(buffer, c.right, offset);
                  },
                  [&](str& s) { memcpy(buffer+offset, s.c_str, s.size); offset += s.size; }
               }, *r);
            }
         }

         inline const concat_t& get_cc(const rope_node* rn)const  {
            return std::get<concat_t>(*rn);
         }

         inline const str& get_str(const rope_node* rn)const {
            return std::get<str>(*rn);
         }
         
         inline const concat_t& get_cc(const std::unique_ptr<rope_node>& rn)const {
            return get_cc(rn.get());
         }

         inline const str& get_str(const std::unique_ptr<rope_node>& rn)const {
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
                            str{ns.c_str, ns.size}), nullptr});
                  _last = c.right.get();
               },
               [](str& s) { check(false, "critical failure should not reach here"); }
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
               [](str& s) { check(false, "critical failure should not reach here"); }
            }, *last);
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
                     concat_t{std::make_unique<rope_node>(str{s,strlen(s)}), nullptr}
                  );
            last = root.get();
            size += strlen(s);
         }
        
         constexpr rope(const rope& r) {
            auto& cc = get_cc(r.root);
            auto& s = get_str(cc.left);
            root = std::make_unique<rope_node>(
                     concat_t{std::make_unique<rope_node>(str{s.c_str, s.size}), nullptr}
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
            str _s{s, len};
            auto new_node = std::make_unique<rope_node>(_s);
         }

         template <size_t N> 
         inline constexpr void append(const char (&s)[N]) {
            append(s, N);
         }

         constexpr void append(const char* s, size_t len) {
            str _s{s, len};
            append(std::make_unique<rope_node>(concat_t{std::make_unique<rope_node>(_s), nullptr}));
            size += len;
         }

         constexpr void append(rope&& r) {
            append(std::move(r.root));
            size += r.size;
         }

         constexpr rope& operator+= (const char* s) {
            append(s, strlen(s));
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
         
         void c_str(char* buffer)const {
            size_t off=0;
            if (root)
               c_str(buffer, root, off);
         }
         

   };

} // ns eosio
