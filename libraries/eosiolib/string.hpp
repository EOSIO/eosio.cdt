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
         
         static constexpr size_t strlen(const char* str) {
            int i=0;
            while (str[i++]);
            return i;
         }

         constexpr void insert(char* buffer, size_t insert_index, const str_t& s)const {
            memmove(buffer+insert_index+s.size, buffer+insert_index, strlen(buffer+insert_index));
            memcpy(buffer+insert_index, s.c_str, s.size);
         }
         
         constexpr size_t copy(char* buffer, size_t index, const str_t&s)const {
            memcpy(buffer+index, s.c_str, s.size);
            return s.size;
         }
         
         constexpr size_t insert_all(char* buffer, size_t offset, size_t inserts_cnt, std::vector<const concat_t*>& inserts )const {
            size_t size_sum = 0;
            for (size_t i=0; i < inserts_cnt && !inserts.empty(); i++) {
               auto ins = inserts.back();
               inserts.pop_back();
               auto& ins_str = std::get<str_t>(*(ins->left));
               insert(buffer, offset+ins->index, ins_str);
               size_sum += ins_str.size;
            }
            return size_sum;
         }  

         void c_str(char* buffer, const rope_node* r)const {
            // the natural recursive version of this function causes call depth maximum assert to be thrown from nodeos
            std::vector<const concat_t*>  inserts;
            std::vector<const rope_node*> last_cc;
            size_t offset = 0;
            const rope_node* tmp = r;
            bool tmp_is_last = false;
            size_t last_offset = 0;
            int inserts_so_far = 0;
            
            while (!tmp_is_last || (tmp_is_last && !last_cc.empty())) {
               if (!tmp || tmp_is_last) {
                  if (!last_cc.empty()) {
                     tmp = last_cc.back();
                     last_cc.pop_back();
                     offset += insert_all(buffer, last_offset, inserts_so_far, inserts);
                     inserts_so_far = 0;
                     last_offset = offset;
                  } else {
                     eosio::check(false, "critical failure, should have reached `last` before nullptr");
                  }
               }
               const auto val = std::get_if<concat_t>(tmp);
               tmp_is_last = tmp == last;

               if (std::holds_alternative<concat_t>(*(val->left))) {
                  last_cc.push_back(val->right);
                  tmp = val->left;
               } else {
                  if (val->index > 0) {
                     inserts.push_back(val);
                     inserts_so_far++;
                  } else {
                     if (auto lval = std::get_if<str_t>(val->left)) {
                        offset += copy(buffer, offset, *lval);
                     }
                  }
                  tmp = val->right;
               }
            }
            insert_all(buffer, last_offset, inserts_so_far, inserts);
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
            if (use_left)
               root = new rope_node(concat_t{root, rn, 0});
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
            append(r.root, true);
            size += r.size;
            last = r.last;
         }
        
         constexpr void append(rope&& r) {
            append(r.root);
            size += r.size;
            last = r.last;
         }

         constexpr rope& operator+= (const char* s) {
            append(s, strlen(s)-1);
            return *this;
         }

         constexpr rope& operator+= (rope&& r) {
            append(std::move(r));
            return *this;
         }

         constexpr rope& operator+= (const rope& r) {
            append(r);
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

         void print() {
            eosio::print(c_str());
         }
         
         constexpr char* c_str()const {
            char* ret = new char[size+1];
            if (root) {
               c_str(ret, root);
            }
            ret[size] = '\0';
            return ret;
         }
   };
} // ns eosio
