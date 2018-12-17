#pragma once

#include "system.hpp"

namespace eosio {
   template <typename T>
   class binary_search_tree {
      private:
         struct Node;
         struct Node {
            Node(T d) :
               left(nullptr), right(nullptr), data(d) 
            {}

            std::unique_ptr<Node> left = nullptr;
            std::unique_ptr<Node> right = nullptr;
            T data;
         };
         std::unique_ptr<Node> root = nullptr;

         void inorder(const std::unique_ptr<Node>& node, std::vector<T>& out_vec)const {
            if (!node)
               return;
            inorder(node->left, out_vec);
            out_vec.push_back(node->data);
            inorder(node->right, out_vec);
         }

         void clone(binary_search_tree& bst, const std::unique_ptr<Node>& node)const {
            if (!node)
               return;
            bst.insert(node->data);
            clone(bst, node->left);
            clone(bst, node->right);
         }
      public: 
         binary_search_tree() {}
         binary_search_tree(std::initializer_list<T> elems) {
            for (auto elem : elems)
               insert(elem);
         }

         std::vector<T> inorder()const {
            std::vector<T> ret_vec;
            inorder(root, ret_vec);
            return ret_vec;
         } 
         
         template <typename U>
         void insert(U&& data) {
            auto new_node = std::make_unique<Node>(data);
            if (!root) {
               root = std::move(new_node);
               return;
            }
            Node* node = root.get();
            Node* last = root.get();
            while (node) {
               last = node;
               if (data < node->data) {
                  node = node->left.get();
                  if (!node)
                     last->left = std::move(new_node);
               }
               else {
                  node = node->right.get();
                  if (!node)
                     last->right = std::move(new_node);
               }
            }
         }

         template <typename U>
         bool has(U&& data)const {
            Node* node = root.get();
            while (node) {
               if (data == node->data)
                  return true;
               else if (data < node->data)
                  node = node->left.get();
               else
                  node = node->right.get();
            }
            return false;
         }
         
         binary_search_tree clone()const {
            binary_search_tree bst;
            clone(bst, root);
            return bst;
         }

         void print()const {
            eosio::print("[");
            std::vector<T> vec = inorder();
            size_t size = vec.size();
            int i = 0;
            for (auto elem : vec) {
               if (++i == size)
                  eosio::print_f("%", elem);
               else
                  eosio::print_f("%,", elem);
            }
            eosio::print("]");
         }
   };
} // namespace eosio
